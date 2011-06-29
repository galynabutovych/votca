/* 
 * File:   jcalc.h
 * Author: james
 *
 * Created on 07 December 2009, 17:19
 */

#ifndef _JCALC_H
#define	_JCALC_H

#include <boost/lexical_cast.hpp>
#include <map>
#include <stdexcept>

#include "crgunittype.h"
#include "votca/tools/tokenizer.h"
#include "crgunit.h"
#include "fock.h"
#include <votca/tools/property.h>

using namespace boost;

class JCalc{

public:

    JCalc() {};
    ~JCalc();
    
    JCalc(string name);

    CrgUnitType * GetCrgUnitTypeByName(string);

    void Initialize(const string &file);
    vector <double> CalcJ (CrgUnit & one, CrgUnit & two);
    int WriteProJ(CrgUnit & one, CrgUnit & two, string namedir=(string("./")) );

    CrgUnit *CreateCrgUnit(int id, const string &type_name, int molid=-1);
    
private:

    struct JCalcData{
            /// the CrgUnitType of the first (will contain coordinates of the still molecules
        CrgUnitType *_type1;
        CrgUnitType *_type2;

        ///variables copied from _type1 and _type2
        pair <vector <unsigned int>, vector <unsigned int> > _orblabels;

        /// the moved coordinates
        mol_and_orb _mol1;
        mol_and_orb _mol2;
        /// the moved orbitals
        orb         _orb1;
        orb         _orb2;
        /// the fock matrix herself
        fock        _fock;

    };
    
    /// To parse the xml file
    Property _options;

    /// a list of charge transport units
    vector <CrgUnitType *> _listCrgUnitType;
    /// a map of charge unit type names to their index in the list
    map <string, CrgUnitType *> _mapCrgUnitByName;
    /// map the fock calculators to the pair of integers representing the corresponding charge unit types
    map <pair<CrgUnitType *, CrgUnitType *> , JCalcData *> _maplistfock;

    /// Enter data for Crg Unit Types
    void ParseCrgUnitTypes(Property &options);
    /// initialise a JCAlcDAta type
    JCalcData * InitJCalcData(CrgUnitType *, CrgUnitType * );
    /// get a J calc data from the map
    JCalcData * getJCalcData(CrgUnit &, CrgUnit &);
};

inline JCalc::JCalc(string name)
{
        Initialize (name);
}

inline CrgUnit *JCalc::CreateCrgUnit(int id, const string &type_name, int molid)
{
    CrgUnitType *type = GetCrgUnitTypeByName(type_name);
    if(!type)
        throw runtime_error("Charge unit type not found: " + type_name);

    return new CrgUnit(id, type, molid);
}

#endif	/* _JCALC_H */

