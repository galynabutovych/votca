/* 
 * Copyright 2009-2011 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __VOTCA_CTP_ATOM_H
#define	__VOTCA_CTP_ATOM_H

#include <string>
#include <assert.h>
#include <votca/tools/types.h>
#include <votca/tools/vec.h>
#include <votca/tools/property.h>




namespace votca { namespace ctp {
using namespace votca::tools;

using namespace std;

class Topology;
class Molecule;
class Segment;
class Fragment;

/**
    \brief information about an atom
 
    The Atom class stores atom id, name, type, mass, charge, residue number
    
*/
class Atom 
{
public:   
    /**
     * constructor
     */
    Atom(Molecule *owner,
         string residue_name,   int resnr,
         string md_atom_name,   int md_atom_id,
         bool hasQMPart,        int qm_atom_id,
         double weight)
       : _mol(owner),
         _resname(residue_name), _resnr(resnr),
         _name(md_atom_name),    _id(md_atom_id),
         _hasQM(hasQMPart),      _qmId(qm_atom_id),
         _weight(weight),        _bPos(false) { }
    
    Atom(int atom_id,   string atom_name)
       : _id(atom_id),  _name(atom_name),
         _hasQM(false), _qmId(-1) { }

    Atom() { };
    /**
     * destructor
     */
   ~Atom() { }

    /**
     * get the id of the atom
     * \return bead id
     */
    const int &getId() const { return _id; }    
    /**
     * get atom name
     * \return atom name
     */
    const string &getName() const { return _name; }    
    /**
     * get the atom type
     * \return atom type 
     */
    const string &getType() const { return _type; }
    /**
     * get the residue number of the atom
     * \return residue id
     */
    const int &getResnr() const { return _resnr; }




    inline void setTopology(Topology *container) { _top = container; }
    inline void setMolecule(Molecule *container) { _mol = container; }
    inline void setSegment(Segment *container)   { _seg = container; }
    inline void setFragment(Fragment *container) { _frag = container; }

    Topology *getTopology() { return _top; }
    Molecule *getMolecule() { return _mol; }
    Segment  *getSegment() { return _seg; }
    Fragment *getFragment() { return _frag; }

    inline void setResnr(const int &resnr) { _resnr = resnr; }
    inline void setResname(const string &resname) { _resname = resname; }
    inline void setWeight(const double &weight) { _weight = weight; }
    inline void setQMPart(const int &qmid) { _hasQM = true; _qmId = qmid; }
    
    inline const int    &getResnr() { return _resnr; }
    inline const string &getResname() { return _resname; }
    inline const double &getWeight() { return _weight; }
    inline const int    &getQMId() { return _qmId; }



    /**
     * get the position of the atom
     * \return atom position
     */
    const vec &getPos() const;
    /**
     * set the position of the atom
     * \param r atom position
     */
    void setPos(const vec &r);
    /**
     * direct access (read/write) to the position of the atom
     * \return reference to position 
     */
    vec &Pos() { return _pos; }
    /** does this configuration store positions? */
    bool HasPos() {return _bPos; }           
    /** dose the bead store a position */
    void HasPos(bool b);


    bool HasQMPart() { return _hasQM; }
    /**
     * molecule the bead belongs to
     * \return Molecule object
     */







protected:
    int         _id;
    string      _name;

    Topology   *_top;
    Molecule   *_mol;
    Segment    *_seg;
    Fragment   *_frag;    

    string      _type;
    int         _resnr;
    string      _resname;
    double      _weight;
    vec         _pos;
    bool        _bPos;

    bool        _hasQM;
    int         _qmId;
        
};

inline void Atom::setPos(const vec &r) {
    _bPos=true;
    _pos = r;
}

inline const vec &Atom::getPos() const {
    assert(_bPos);
    return _pos;
}

inline void Atom::HasPos(bool b) {
    _bPos=b;
}

}}

#endif	/* __VOTCA_CTP_ATOM_H */

