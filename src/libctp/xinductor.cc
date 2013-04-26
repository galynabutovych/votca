#include <votca/ctp/xinductor.h>


namespace votca { namespace ctp {


void XInductor::Evaluate(XJob *job, 
                         vector< Segment* >            &segsPolSphere,
                         vector< Segment* >            &segsOutSphere,
                         vector< vector<APolarSite*> > &polsPolSphere,
                         vector< vector<APolarSite*> > &polsOutSphere,
                         vector< vector<APolarSite*> > &polarSites,
                         vector< vector<APolarSite*> > &polarSites_job) {    
    
    // Copy containers
    _job            = job;
    _qm0            = job->getPolarTop()->QM0();
    _mm1            = job->getPolarTop()->MM1();
    _mm2            = job->getPolarTop()->MM2();
    _segsPolSphere  = segsPolSphere;
    _segsOutSphere  = segsOutSphere;
    _polsPolSphere  = polsPolSphere;
    _polsOutSphere  = polsOutSphere;
    _polarSites     = polarSites;
    _polarSites_job = polarSites_job;
    
    // Check shell definition
    this->WriteShellPdb();

    // ++++++++++++++++++++++++++ //
    // (De-)polarize, charge to N //
    // ++++++++++++++++++++++++++ //

    if (job->StartFromCPT()) {

        if (this->_maverick) {
            cout << endl
                 << "... ... ... Loading induced dipoles from .cpt file. "
                 << flush;
        }
        assert(false); // Load induced dipole moments from file
    }

    else {

        vector< vector<APolarSite*> > ::iterator sit;
        vector< APolarSite* >         ::iterator pit;

        // Depolarize inner sphere
        for (sit = _polsPolSphere.begin(); sit < _polsPolSphere.end(); ++sit) {
        for (pit = (*sit).begin(); pit < (*sit).end(); ++pit) {
            (*pit)->Depolarize();
            (*pit)->Charge(0); // <- Not necessarily neutral state
        }}

        // Depolarize outer shell
        for (sit = _polsOutSphere.begin(); sit < _polsOutSphere.end(); ++sit) {
        for (pit = (*sit).begin(); pit < (*sit).end(); ++pit) {
            (*pit)->Depolarize();
            (*pit)->Charge(0); // <- Not necessarily neutral state
        }}
    }
    
    // +++++++++++++++++ //
    // Induction workers //
    // +++++++++++++++++ //

    for (int id = 0; id < this->_subthreads; ++id) {
        InduWorker *newIndu = new InduWorker(id,_top,this);
        _indus.push_back(newIndu);
        newIndu->InitSpheres(&_segsPolSphere,&_segsOutSphere,
                             &_polsPolSphere,&_polsOutSphere);
        newIndu->SetSwitch(1);
    }
    
    this->InitChunks();

    // ++++++++++++++++++++++++++ //
    // Compute state energy       //
    // ++++++++++++++++++++++++++ //

    double  E_state  = 0.0;
    int     iter     = 0;
    int     state    = 0;

    if (this->_induce) iter      = this->Induce(state, job);
    if (this->_induce) E_state   = this->Energy(state, job);
    else               E_state   = this->EnergyStatic(state, job);

    job->setIter(iter);
    
}


int XInductor::Induce(int state, XJob *job) {
    
    for (int id = 0; id < this->_subthreads; ++id) {
        _indus[id]->SetSwitch(1);
    }

    double wSOR = (state == 0) ? this->_wSOR_N : this->_wSOR_C;
    double eTOL = this->_epsTol;
    int    maxI = this->_maxIter;

    // Intra-pair induction ...
    bool   induce_intra_pair = this->_induce_intra_pair;
    // ... change this for jobs of type "site":
    if (job->getSegments().size() == 1) { induce_intra_pair = true; }

    vector< vector<APolarSite*> > ::iterator sit1;
    vector< vector<APolarSite*> > ::iterator sit2;
    vector< APolarSite* > ::iterator pit1;
    vector< APolarSite* > ::iterator pit2;
    vector< Segment* > ::iterator seg1;
    vector< Segment* > ::iterator seg2;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++ //
    // Inter-site fields (arising from perm. m'poles) //
    // ++++++++++++++++++++++++++++++++++++++++++++++ //
    
    for (sit1 = _polsPolSphere.begin(), seg1 = _segsPolSphere.begin();
         sit1 < _polsPolSphere.end();
         ++sit1, ++seg1) {
    for (sit2 = sit1 + 1, seg2 = seg1 + 1;
         sit2 < _polsPolSphere.end();
         ++sit2, ++seg2) {

        // Intra-pair permanent induction field?
         if ( !induce_intra_pair ) {
             if ( job->isInCenter((*seg1)->getId())
               && job->isInCenter((*seg2)->getId()) ) {
                 continue;
             }
         }

         for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
         for (pit2 = (*sit2).begin(); pit2 < (*sit2).end(); ++pit2) {

             _actor.FieldPerm(*(*pit1), *(*pit2));
         }}
    }}
    
    // Permanent fields generated by outer shell    
    // (Outer shell itself is treated as non-polarizable)
    for (sit1 = _polsPolSphere.begin();
         sit1 < _polsPolSphere.end();
         ++sit1) {
    for (sit2 = _polsOutSphere.begin();
         sit2 < _polsOutSphere.end();
         ++sit2) {

         for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
         for (pit2 = (*sit2).begin(); pit2 < (*sit2).end(); ++pit2) {

             _actor.FieldPerm(*(*pit1), *(*pit2));
         }}
    }}
    

    // +++++++++++++++++++ //
    // 1st-order induction //
    // +++++++++++++++++++ //

    if (!job->StartFromCPT()) { // OVERRIDE
        for (sit1 = _polsPolSphere.begin();
             sit1 < _polsPolSphere.end();
             ++sit1) {

             for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
                 (*pit1)->InduceDirect();
             }
        }
    }
    else {
        assert(false); // Load induced dipole moments from file
    }



    // ++++++++++++++++++++++ //
    // Higher-order induction //
    // ++++++++++++++++++++++ //

    
    int iter = 0;
    for ( ; iter < maxI; ++iter) {

        // Reset fields FUx, FUy, FUz
        for (sit1 = _polsPolSphere.begin();
             sit1 < _polsPolSphere.end();
             ++sit1) {

            for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
                (*pit1)->ResetFieldU();
            }
        }

        // Intra-site contribution to induction field
        for (sit1 = _polsPolSphere.begin();
             sit1 < _polsPolSphere.end();
             ++sit1) {

            for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
            for (pit2 = pit1 + 1;        pit2 < (*sit1).end(); ++pit2) {

                _actor.FieldIndu(*(*pit1),*(*pit2));                            
            }}
        }

        // Inter-site contribution to induction field

//        for (sit1 = _polsPolSphere.begin();
//             sit1 < _polsPolSphere.end();
//             ++sit1) {
//        for (sit2 = sit1 + 1;
//             sit2 < _polsPolSphere.end();
//             ++sit2) {
//
//            for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
//            for (pit2 = (*sit2).begin(); pit2 < (*sit2).end(); ++pit2) {
//
//                _actor.FieldIndu(*(*pit1), *(*pit2));
//            }}
//        }}

        for (int id = 0; id < this->_subthreads; ++id) {
            _indus[id]->Start();
        }

        for (int id = 0; id < this->_subthreads; ++id) {
            _indus[id]->WaitDone();
        }

        this->ClearTodoTable();



        // Induce again
        for (sit1 = _polsPolSphere.begin();
             sit1 < _polsPolSphere.end();
             ++sit1) {

             for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
                 (*pit1)->Induce(wSOR);                                         
             }
        }

        // Check for convergence
        bool converged = true;
        double maxdU = -1;
        double avgdU = 0.0;
        int    baseN = 0;
        for (sit1 = _polsPolSphere.begin();
             sit1 < _polsPolSphere.end();
             ++sit1) {

             for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
                 double dU = (*pit1)->HistdU();
                 avgdU += dU;
                 ++baseN;
                 if ( dU > maxdU ) { maxdU = dU; }
                 if ( dU > eTOL ) { converged = false; }
             }
        }
        avgdU /= baseN;
        if (avgdU < eTOL/10.) { converged = true; }

//        cout << " | MAX dU " << maxdU
//             << " | AVG dU " << avgdU
//             << " | SOR " << wSOR << flush;

        // Break if converged
        if      (converged) {
            break;
        }
        else if (iter == maxI - 1) {
            //this->_master->LockCout();
            cout << endl << "... ... ... WARNING Induced multipoles for job "
                 << job->getId() << " did not converge to precision: "
                 << " AVG dU:U " << avgdU << flush;
            //this->_master->UnlockCout();
            break;
        }
    }

    return iter;

}



double XInductor::Energy(int state, XJob *job) {

    double int2eV = 1/(4*M_PI*8.854187817e-12) * 1.602176487e-19 / 1.000e-9;    

    _actor.ResetEnergy();
    
    // Energy splittings =======================================================
    // PAIR/SITE        <->        SPH1         <->          SPH2 = OUT       //
    double E_Tot = 0.0;
    // ... 0th kind    
    double E_Pair_Pair = 0.0;    
    double E_Pair_Sph1 = 0.0;
    double E_Sph1_Sph1 = 0.0;    
    double E_Pair_Sph2 = 0.0;
    double E_Sph1_Sph2 = 0.0;
    // ... 1st kind
    double eu_inter = 0.0;
    double eu_intra = 0.0;
    double e_perm   = 0.0;
    // ... 2nd kind
    double epp      = 0.0;
    double epu      = 0.0;
    double euu      = 0.0;
    // ... 3rd kind
    double e_f_c_c          = 0.0;
    double e_f_c_non_c      = 0.0;
    double e_f_c_out        = 0.0;
    double e_f_non_c_non_c  = 0.0;   
    double e_f_non_c_out    = 0.0;
    double e_m_c            = 0.0;
    double e_m_c_out        = 0.0;
    double e_m_non_c        = 0.0;
    double e_m_non_c_out    = 0.0;
    double e_m_out          = 0.0;
    // =========================================================================

    vector< Segment* >               ::iterator      seg1;
    vector< Segment* >               ::iterator      seg2;
    vector< vector<APolarSite*> >    ::iterator      sit1;
    vector< vector<APolarSite*> >    ::iterator      sit2;
    vector< APolarSite* >            ::iterator      pit1;
    vector< APolarSite* >            ::iterator      pit2;

    
    // =============================================================== //
    // System Energy | QM | MM1 | MM2 |                                //
    // =============================================================== //
    

    for (int id = 0; id < this->_subthreads; ++id) {
        _indus[id]->SetSwitch(0);
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
    // Inter-site energy comprising central + first polarization shell //
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //


    for (int id = 0; id < this->_subthreads; ++id) {
        _indus[id]->Start();
    }

    for (int id = 0; id < this->_subthreads; ++id) {
        _indus[id]->WaitDone();
    }

    for (int id = 0; id < this->_subthreads; ++id) {
        E_Pair_Pair += _indus[id]->GetEPairPair();
        E_Pair_Sph1 += _indus[id]->GetEPairSph1();
        E_Sph1_Sph1 += _indus[id]->GetESph1Sph1();

        eu_inter += _indus[id]->GetActor().getEU_INTER();
        eu_intra += _indus[id]->GetActor().getEU_INTRA();
        e_perm   += _indus[id]->GetActor().getEP();

        epp += _indus[id]->GetActor().getEPP();
        epu += _indus[id]->GetActor().getEPU();
        euu += _indus[id]->GetActor().getEUU();

        e_f_c_c             += _indus[id]->GetE_f_C_C();
        e_f_c_non_c         += _indus[id]->GetE_f_C_non_C();
        e_f_non_c_non_c     += _indus[id]->GetE_f_non_C_non_C();            
        e_m_c               += _indus[id]->GetE_m_C();
        e_m_non_c           += _indus[id]->GetE_m_non_C();
    }

    this->ClearTodoTable(); 


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
    // Inter-site energy resulting from interaction with static shell  //
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

    // Interaction between central and static shell
    for (int i = 0; i < job->getSegments().size(); ++i) {

        vector< APolarSite* > central 
                = _polarSites_job[ job->getSegments()[i]->getId() - 1 ];

        for (sit1 = _polsOutSphere.begin(); 
             sit1 < _polsOutSphere.end(); ++sit1) {
            for (pit1 = (*sit1).begin(); 
                 pit1 < (*sit1).end(); ++pit1) {
                for (pit2 = central.begin(); 
                     pit2 < central.end(); ++pit2) {
                    e_f_c_out += _actor.E_f(*(*pit1), *(*pit2));
                    e_m_c_out += _actor.E_m(*(*pit2), *(*pit1));
                }
            }
        }
    }

    // Interaction between polarizable and static shell
    for (sit1 = this->_polsOutSphere.begin(); 
         sit1 < _polsOutSphere.end(); ++sit1) {
    for (sit2 = this->_polsPolSphere.begin(), 
         seg2 = this->_segsPolSphere.begin(); 
         sit2 < _polsPolSphere.end(); ++sit2, ++seg2) {

        // Continue when hitting one of the central sites (already covered)
        if ( job->isInCenter((*seg2)->getId()) ) {
            continue;
        }

        for (pit1 = (*sit1).begin(); pit1 < (*sit1).end(); ++pit1) {
        for (pit2 = (*sit2).begin(); pit2 < (*sit2).end(); ++pit2) {
            e_f_non_c_out += _actor.E_f(*(*pit1), *(*pit2));
            e_m_non_c_out += _actor.E_m(*(*pit2), *(*pit1));
        }}
    }}


    // Increment energies
    // ... 0th kind        
    E_Pair_Sph2 += e_f_c_out + e_m_c_out;
    E_Sph1_Sph2 += e_f_non_c_out + e_m_non_c_out;
    // ... 1st kind
    e_perm      += _actor.getEP();
    eu_inter    += _actor.getEU_INTER();
    // ... 2nd kind
    epp += _actor.getEPP();
    epu += _actor.getEPU();
    euu += _actor.getEUU();
    // ... 3rd kind
    // ... ... -> done in loop above, but need to summarize e_m_*
    e_m_c      += e_m_c_out;
    e_m_non_c  += e_m_non_c_out;


    E_Tot = E_Pair_Pair + E_Pair_Sph1 + E_Sph1_Sph1 + E_Pair_Sph2;
        


    // =============================================================== //
    // Energy Output                                                   //
    // =============================================================== //

    // ... 0th kind
    E_Tot = E_Pair_Pair
          + E_Pair_Sph1
          + E_Sph1_Sph1
          + E_Pair_Sph2
          + E_Sph1_Sph2;

    if (this->_maverick) {
        cout << endl << "... ... ... ... "
             << "E(" << state << ") = " << E_Tot * int2eV << " eV "
             << endl << "                     = (Site, Site) " << E_Pair_Pair * int2eV
             << endl << "                     + (Site, Sph1) " << E_Pair_Sph1 * int2eV
             << endl << "                     + (Sph1, Sph1) " << E_Sph1_Sph1 * int2eV
             << endl << "                     + (Site, Sph2) " << E_Pair_Sph2 * int2eV
             << endl << "                     + (Sph1, Sph2) " << E_Sph1_Sph2 * int2eV
             << flush;
    }

    // ... 1st kind
    double E_PPUU = epp 
                  + epu 
                  + euu;

    if (this->_maverick) {
        cout << endl
             << "... ... ... ... E(" << state << ") = " << E_PPUU * int2eV
             << " eV " 
             << endl << "                     = (PP) "    << epp  * int2eV
             << endl << "                     + (PU) "    << epu  * int2eV
             << endl << "                     + (UU) "    << euu  * int2eV
             << flush;
    }

    // ... 2nd kind
    double E_f_m = e_f_c_c 
                 + e_f_c_non_c
                 + e_f_c_out 
                 + e_f_non_c_non_c 
                 + e_f_non_c_out
                 + e_m_c 
                 + e_m_non_c
                 + e_m_out;

    if (this->_maverick) {
        cout << endl
             << "... ... ... ... E(" << state << ") = " << E_f_m * int2eV
             << " eV " 
             << endl << "                     = (f,0-0) " << e_f_c_c          * int2eV
             << endl << "                     + (f,0-1) " << e_f_c_non_c      * int2eV
             << endl << "                     + (f,0-2) " << e_f_c_out        * int2eV
             << endl << "                     + (f,1-1) " << e_f_non_c_non_c  * int2eV
             << endl << "                     + (f,1-2) " << e_f_non_c_out    * int2eV
             << endl << "                     + (m,-0-) " << e_m_c            * int2eV
             << endl << "                     + (m,-1-) " << e_m_non_c        * int2eV
             << endl << "                     + (m,-2-) " << e_m_out          * int2eV
             << flush;
    }

    // Forward results to job
    job->setEnergy(E_Tot            *int2eV,           
                   E_Pair_Pair      *int2eV,
                   E_Pair_Sph1      *int2eV,
                   E_Pair_Sph2      *int2eV, 
                   E_Sph1_Sph1      *int2eV,
                   E_Sph1_Sph2      *int2eV,                       
                   e_perm           *int2eV,
                   eu_inter         *int2eV);

    job->setEnergy_PPUU(epp         *int2eV,
                        epu         *int2eV,
                        euu         *int2eV);

    job->setEnergy_f_m(e_f_c_c         *int2eV,
                       e_f_c_non_c     *int2eV,
                       e_f_c_out       *int2eV,
                       e_f_non_c_non_c *int2eV, 
                       e_f_non_c_out   *int2eV,
                       e_m_c           *int2eV, 
                       e_m_non_c       *int2eV,
                       e_m_out         *int2eV);

    return E_Tot;
}


double XInductor::EnergyStatic(int state, XJob *job) {
    
    double int2eV = 1/(4*M_PI*8.854187817e-12) * 1.602176487e-19 / 1.000e-9;

    _actor.ResetEnergy();
    
    // Energy splittings =======================================================
    // PAIR/SITE        <->        SPH1         <->          SPH2 = OUT       //
    double E_Tot = 0.0;
    // ... 0th kind    
    double E_Pair_Pair = 0.0;    
    double E_Pair_Sph1 = 0.0;
    double E_Sph1_Sph1 = 0.0;    
    double E_Pair_Sph2 = 0.0;
    double E_Sph1_Sph2 = 0.0;
    // ... 1st kind
    double eu_inter = 0.0;
    double eu_intra = 0.0;
    double e_perm   = 0.0;
    // ... 2nd kind
    double epp      = 0.0;
    double epu      = 0.0;
    double euu      = 0.0;
    // ... 3rd kind
    double e_f_c_c          = 0.0;
    double e_f_c_non_c      = 0.0;
    double e_f_c_out        = 0.0;
    double e_f_non_c_non_c  = 0.0;   
    double e_f_non_c_out    = 0.0;
    double e_m_c            = 0.0;
    double e_m_c_out        = 0.0;
    double e_m_non_c        = 0.0;
    double e_m_non_c_out    = 0.0;
    double e_m_out          = 0.0;
    // =========================================================================

    vector< Segment* >               ::iterator      seg1;
    vector< Segment* >               ::iterator      seg2;
    vector< vector<APolarSite*> >    ::iterator      sit1;
    vector< vector<APolarSite*> >    ::iterator      sit2;
    vector< APolarSite* >            ::iterator      pit1;
    vector< APolarSite* >            ::iterator      pit2;

        
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
    // Interaction pair <-> inner cut-off, without intra-pair interaction //
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

    for (int i = 0; i < job->getSegments().size(); ++i) {

        vector<APolarSite*> central
                = _polarSites_job[ job->getSegments()[i]->getId() - 1 ];

        for (seg1 = _segsPolSphere.begin(); 
             seg1 < _segsPolSphere.end(); ++seg1) {

            int id = (*seg1)->getId();

            if (job->isInCenter(id)) { continue; }

            for (pit1 = _polarSites_job[id-1].begin();
                 pit1 < _polarSites_job[id-1].end(); ++pit1) {
                for (pit2 = central.begin();
                     pit2 < central.end(); ++pit2) {

                    e_f_c_non_c += _actor.E_f(*(*pit1), *(*pit2));                        
                }
            }                
        }


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
    // Interaction pair <-> outer cut-off                                 //
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

        for (seg1 = _segsOutSphere.begin(); 
             seg1 < _segsOutSphere.end(); ++seg1) {

            int id = (*seg1)->getId();

            if (job->isInCenter(id)) {
                throw std::runtime_error("This should not have happened.");
            }

            for (pit1 = _polarSites_job[id-1].begin();
                 pit1 < _polarSites_job[id-1].end();
                 ++pit1) {
                for (pit2 = central.begin();
                     pit2 < central.end();
                     ++pit2) {

                     e_f_c_out += _actor.E_f(*(*pit1), *(*pit2));
                }
            }
        }
    } /* Finish loop over segments in central sphere */


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
    // Intra-pair interaction                                             //
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

    for (int i = 0; i < job->getSegments().size(); ++i) {

        vector<APolarSite*> central_i
                = _polarSites_job[ job->getSegments()[i]->getId() - 1 ];

    for (int j = i+1; j < job->getSegments().size(); ++j) {

        vector<APolarSite*> central_j
                = _polarSites_job[ job->getSegments()[j]->getId() - 1 ];

        for (pit1 = central_i.begin();
             pit1 < central_i.end(); ++pit1) {
        for (pit2 = central_j.begin();
             pit2 < central_j.end(); ++pit2) {

            e_f_c_c += _actor.E_f(*(*pit1), *(*pit2));
        }}

    }}


    // Increment energies
    // ... 0th kind
    E_Pair_Pair     += e_f_c_c;
    E_Pair_Sph1     += e_f_c_non_c;
    E_Pair_Sph2     += e_f_c_out;
    // ... 1st kind
    e_perm          += _actor.getEP();
    eu_inter        += _actor.getEU_INTER();
    // ... 2nd kind
    epp             += _actor.getEPP();
    epu             += _actor.getEPU();
    euu             += _actor.getEUU();
    // ... 3rd kind
    // ... ... -> done in loops above        


    // =============================================================== //
    // Energy Output                                                   //
    // =============================================================== //

    // ... 0th kind
    E_Tot = E_Pair_Pair
          + E_Pair_Sph1
          + E_Sph1_Sph1
          + E_Pair_Sph2
          + E_Sph1_Sph2;

    if (this->_maverick) {
        cout << endl << "... ... ... ... "
             << "E(" << state << ") = " << E_Tot * int2eV << " eV "
             << endl << "                     = (Site, Site) " << E_Pair_Pair * int2eV
             << endl << "                     + (Site, Sph1) " << E_Pair_Sph1 * int2eV
             << endl << "                     + (Sph1, Sph1) " << E_Sph1_Sph1 * int2eV
             << endl << "                     + (Site, Sph2) " << E_Pair_Sph2 * int2eV
             << endl << "                     + (Sph1, Sph2) " << E_Sph1_Sph2 * int2eV
             << flush;
    }

    // ... 1st kind
    double E_PPUU = epp 
                  + epu 
                  + euu;

    if (this->_maverick) {
        cout << endl
             << "... ... ... ... E(" << state << ") = " << E_PPUU * int2eV
             << " eV " 
             << endl << "                     = (PP) "    << epp  * int2eV
             << endl << "                     + (PU) "    << epu  * int2eV
             << endl << "                     + (UU) "    << euu  * int2eV
             << flush;
    }

    // ... 2nd kind
    double E_f_m = e_f_c_c 
                 + e_f_c_non_c
                 + e_f_c_out 
                 + e_f_non_c_non_c 
                 + e_f_non_c_out
                 + e_m_c 
                 + e_m_non_c
                 + e_m_out;

    if (this->_maverick) {
        cout << endl
             << "... ... ... ... E(" << state << ") = " << E_f_m * int2eV
             << " eV " 
             << endl << "                     = (f,0-0) " << e_f_c_c          * int2eV
             << endl << "                     + (f,0-1) " << e_f_c_non_c      * int2eV
             << endl << "                     + (f,0-2) " << e_f_c_out        * int2eV
             << endl << "                     + (f,1-1) " << e_f_non_c_non_c  * int2eV
             << endl << "                     + (f,1-2) " << e_f_non_c_out    * int2eV
             << endl << "                     + (m,-0-) " << e_m_c            * int2eV
             << endl << "                     + (m,-1-) " << e_m_non_c        * int2eV
             << endl << "                     + (m,-2-) " << e_m_out          * int2eV
             << flush;
    }

    // Forward results to job
    job->setEnergy(E_Tot            *int2eV,           
                   E_Pair_Pair      *int2eV,
                   E_Pair_Sph1      *int2eV,
                   E_Pair_Sph2      *int2eV, 
                   E_Sph1_Sph1      *int2eV,
                   E_Sph1_Sph2      *int2eV,                       
                   e_perm           *int2eV,
                   eu_inter         *int2eV);

    job->setEnergy_PPUU(epp         *int2eV,
                        epu         *int2eV,
                        euu         *int2eV);

    job->setEnergy_f_m(e_f_c_c         *int2eV,
                       e_f_c_non_c     *int2eV,
                       e_f_c_out       *int2eV,
                       e_f_non_c_non_c *int2eV, 
                       e_f_non_c_out   *int2eV,
                       e_m_c           *int2eV, 
                       e_m_non_c       *int2eV,
                       e_m_out         *int2eV);    
    
    return E_Tot;
}


}}