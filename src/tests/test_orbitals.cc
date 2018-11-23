/*
 * Copyright 2009-2018 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
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
#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE orbitals_test
#include <boost/test/unit_test.hpp>
#include <votca/xtp/convergenceacc.h>
#include <votca/xtp/orbitals.h>
#include <fstream>
#include "votca/xtp/aomatrix.h"

using namespace votca::xtp;
using std::endl;
BOOST_AUTO_TEST_SUITE(orbitals_test)
BOOST_AUTO_TEST_CASE(readxyztest){
  std::ofstream xyzfile("molecule.xyz");
  xyzfile << " C 0.0 3 1" << std::endl;
  xyzfile << " methane" << std::endl;
  xyzfile << " C            .000000     .000000     .000000" << std::endl;
  xyzfile << " H            .629118     .629118     .629118" << std::endl;
  xyzfile << " H           -.629118    -.629118     .629118" << std::endl;
  xyzfile << " H            .629118    -.629118    -.629118" << std::endl;
  xyzfile << " H           -.629118     .629118    -.629118" << std::endl;
  xyzfile.close();
  
  bool errorhappen=false;
  Orbitals orb;
  try{
    orb.LoadFromXYZ("molecule.xyz");
  }catch (const std::runtime_error& error)
{
    std::cout<<error.what()<<std::endl;
    errorhappen=true;
  }
  
   BOOST_CHECK_EQUAL(errorhappen, true);
}

BOOST_AUTO_TEST_CASE(sortEnergies){
  
  Orbitals orb;
  Eigen::VectorXd Energies=Eigen::VectorXd::LinSpaced(10,-5,5);
  Eigen::VectorXd switched=Energies;
  switched(3)=Energies(5);
  switched(5)=Energies(3);
  orb.MOEnergies()=switched;
  orb.MOCoefficients()=Eigen::MatrixXd::Zero(10,10);
  orb.OrderMOsbyEnergy();
  bool issorted=Energies.isApprox(orb.MOEnergies(),0.001);
  if(!issorted){
    std::cout<<"before"<<std::endl;
    std::cout<<Energies<<std::endl;
    std::cout<<"after"<<std::endl;
    std::cout<<orb.MOEnergies()<<std::endl;
  }
  BOOST_CHECK_EQUAL(issorted, true);
}


BOOST_AUTO_TEST_CASE(densmat_test) {
  
  Orbitals orb;
  orb.setBasisSetSize(17);
  orb.setNumberOfLevels(4,12);
  orb.setBSEindices(0,9,2);
  orb.setNumberOfElectrons(5);

  orb.MOCoefficients()=Eigen::MatrixXd::Zero(17,17);
  orb.MOCoefficients()<<0.9907442950821396,0.20551857477099006,1.7023418853572578E-4,9.074399061719398E-5,3.837098252664792E-5,0.16974897759501614,-4.204899244661065E-5,7.615775441246816E-5,4.676066062450439E-5,2.884698428033532E-5,-4.481545968117419E-5,-1.3204180988296866E-4,-0.11680388320125862,1.6189605485598983E-5,-5.371063226647223E-6,-1.2047015045305022E-4,0.023240774591255682,
0.08911047513015412,-0.20355821452675385,-2.4242411849295854E-4,-1.389669272938938E-4,-6.0728340934352946E-5,-0.07548903518938142,3.262334625557027E-5,-7.10564649816358E-5,-9.986644282246894E-5,1.6702170747853146E-4,-3.4464448598096653E-4,-0.0011237781000302065,0.16022562161339227,2.5597538884079596E-4,-5.987456557611593E-4,0.0010289053314875896,1.9979502865114958,
8.107959419149133E-6,-1.3746554617574666E-4,0.30409982472519304,-0.04943981412502669,-0.18275903305786392,-1.4404163587992467E-4,0.14257881572584336,-0.032034049310960475,0.23956240448089597,0.5387878318279876,0.07398471014538607,0.4387442141244202,-0.0019929701691466604,-0.4478311747397298,0.10346528783585213,0.7800429786477285,4.413287003871695E-4,
1.1754110374944727E-5,-1.6786131622203274E-4,0.16187208368047987,0.2472771093038189,0.202328179665061,-3.820994854847884E-4,-0.15977453182924237,0.19628611211192631,0.1213197329408795,-0.4388814545469389,0.20281124109383114,0.5046453462735411,-0.002238807774688505,0.5150776559618175,-0.6393382963354479,0.38099226873671443,6.6047695407522E-4,
-5.048302992539679E-7,1.3568022971562862E-5,-0.0982741302898521,0.2543655402180741,-0.23220810764458472,-1.944169891214517E-5,0.18144825026251918,0.1980251300437136,-0.08147866661574833,0.07405511360833358,0.6646693464629595,-0.2028284384893438,2.279342030052545E-4,-0.5940706293255381,-0.6323141290039537,-0.25763397034652824,-2.315661453010646E-5,
-0.08143505806634609,-0.7045679157387013,-4.4787955662640034E-4,-2.0910866005311433E-4,-8.259360481466638E-5,-2.424093867046151,6.780952279118335E-4,-0.0011263648445154907,-2.1613578613839522E-4,-3.6586704890798056E-4,3.4830029266647066E-4,7.405837519085355E-4,0.14778031305572506,-5.874372031528609E-4,0.0012517213522246784,-0.0016624970087244567,-3.9307290475800984,
-4.845180411930193E-5,-3.15232661550629E-4,0.29033356022603657,-0.047214119469099654,-0.1746229236165066,-0.0010623554092176837,0.7157746323727515,-0.1610150829224024,1.207129610424005,-0.8861441619621693,-0.12082122795238953,-0.7253976412392744,0.004702513046801041,0.5445150195359842,-0.12577564486773046,-0.9479104485435895,-0.0029133750584969995,
-6.97620867722761E-5,-4.518653975073802E-4,0.1545537407856724,0.23619223591899627,0.19332335260823175,-0.0022069712738471844,-0.8019796219162296,0.9866731245664445,0.6112535135056467,0.7197858303062914,-0.33414367829784547,-0.8313511890447793,0.005950677113076908,-0.6262908623920043,0.7772100698368285,-0.46298826705698615,-0.004212137095734844,
3.025513825093562E-6,2.01122633970492E-5,-0.09381992712564609,0.24295366644273125,-0.22187182348758724,-4.4849221276044244E-5,0.9107435069506925,0.9953611683731745,-0.410462654769864,-0.11998173280053394,-1.0921777637934424,0.3342503817907009,-4.2739718665356585E-4,0.7223406094651171,0.7686726934386922,0.313084531007334,1.7805853565722838E-4,
7.37158442829733E-4,-0.11947916702565467,0.0709877748234037,0.19486216169399248,-0.19963587525346332,0.021970191718328234,-0.08852720779007611,-0.08752682338800259,-0.029183087590095056,0.2404775527159037,0.6601558927534981,0.12659420741971703,-0.667743162629014,0.6085237963794983,0.5985008552792193,-0.20986989221784247,0.15058182893557184,
0.01608075222828349,0.010011975370567302,0.05470769366508042,0.15064492298573015,-0.15457287617332643,0.9255720960213428,-1.206121064891608,-1.1996011487723761,-0.4056978894599689,-0.05298499024971882,-0.14303294036061362,-0.02485325238033535,0.2987439256438313,-1.013863139043371,-0.9971897529370147,0.3487761135053009,0.7790198218470478,
7.138422212641049E-4,-0.1196094899026972,-0.2842166232364692,-0.04556245465364824,-0.007053279485955037,0.02230987096947848,-0.0036852600571774035,0.023565132654508568,0.12524632931555021,-0.17382906342060087,-0.060853394762126325,-0.6918002740597844,-0.6669226960233403,0.035864677061268974,-0.16556444794734262,0.861747976464038,0.14795843313851584,
0.016063982425839392,0.009859499054321354,-0.21897014622303052,-0.035202371814191405,-0.005465437420629127,0.9257581110182598,-0.04980711970695707,0.32109962848034435,1.7221001218391445,0.035905784702012784,0.013498840982845527,0.14562342227832253,0.3047901425448661,-0.05951259424946384,0.27553962984145547,-1.4351881046889507,0.7788885658103283,
7.409253682701713E-4,-0.11944350976971661,0.07513719426852992,0.09761819951158109,0.26015661269895035,0.021951308529804293,0.11609692884630969,-0.043567065850759326,-0.03144638449326769,-0.5763047315656864,-0.12654072032163471,0.40137211030812026,-0.6681794172539768,-0.7963825251648803,0.30840285525063804,-0.2072016617793105,0.15099996101012425,
0.016080755519097423,0.01002674093860076,0.05792745695939718,0.07547359788405249,0.20146343358941035,0.9257451411073216,1.5832800173788046,-0.5979818102877212,-0.4371140280860898,0.12657732301631605,0.028622969925985273,-0.08503838333777329,0.2984829177687489,1.3272620325313798,-0.5139436624743582,0.3442485273058088,0.7788837381362927,
7.28527937995282E-4,-0.11955381155867925,0.13824691197079378,-0.24681524845383587,-0.05342022280053824,0.02203103650716836,-0.024102851640929047,0.10805548656383365,-0.06369358394744931,0.5104340186250341,-0.47455391846866973,0.15780550183151598,-0.6668890690900623,0.15206929191456245,-0.740817337705803,-0.4483567934721803,0.14962178851412328,
0.016079430040486366,0.00997356912486891,0.10652865972788295,-0.19075159916388634,-0.04134322428836118,0.9252734630049098,-0.3278217219514359,1.4769403455748562,-0.8808246951973489,-0.1097870975651871,0.10218284493845689,-0.03092608243612813,0.2996873524742791,-0.25325921205821206,1.2339194146754904,0.7458659447098919,0.7792524302192918;
  


  QMState s=QMState("n");
  Eigen::MatrixXd dmat_gs=orb.DensityMatrixFull(s);

  Eigen::MatrixXd dmat_gs_ref=Eigen::MatrixXd::Zero(17,17);
  dmat_gs_ref<<2.04762,0.0929013,5.4126e-05,5.42835e-05,1.72817e-05,-0.450966,-0.000135298,-0.00022848,2.64124e-05,-0.0475902,0.0360251,-0.0478545,0.0358023,-0.0475843,0.0360186,-0.0476952,0.0359624,
0.0929013,0.0987534,-7.62918e-05,-7.67761e-05,-2.86625e-05,0.272328,-7.94404e-06,3.09478e-05,-2.96854e-05,0.0486847,-0.00127851,0.0489727,-0.00103507,0.0486959,-0.00126519,0.0488037,-0.00119335,
5.4126e-05,-7.62918e-05,0.189842,0.0739999,-0.0849219,-5.93365e-05,0.181249,0.0706451,-0.0810844,0.0239397,0.018375,-0.168322,-0.129699,0.0360788,0.0277662,0.10852,0.0836497,
5.42835e-05,-7.67761e-05,0.0739999,0.174697,0.0939819,-1.3789e-05,0.070644,0.166846,0.0897801,0.119392,0.0922104,-0.114506,-0.0883027,0.0726428,0.0560765,-0.0772667,-0.0598519,
1.72817e-05,-2.86625e-05,-0.0849219,0.0939819,0.148719,-3.73871e-05,-0.0810839,0.0897811,0.142038,0.0851767,0.0658853,0.03268,0.0251299,0.0348901,0.0270105,-0.152738,-0.117979,
-0.450966,0.272328,-5.93365e-05,-1.3789e-05,-3.73871e-05,1.0061,0.000211774,0.000410879,-4.64011e-05,0.168097,-0.0168393,0.168703,-0.0162988,0.168083,-0.0168316,0.168328,-0.0166886,
-0.000135298,-7.94404e-06,0.181249,0.070644,-0.0810839,0.000211774,0.173046,0.0674414,-0.0774198,0.022895,0.017534,-0.160658,-0.123832,0.034487,0.0265019,0.103657,0.0798622,
-0.00022848,3.09478e-05,0.0706451,0.166846,0.0897811,0.000410879,0.0674414,0.159348,0.0857671,0.114101,0.0880616,-0.109268,-0.0843255,0.0694466,0.0535471,-0.0737506,-0.0571905,
2.64124e-05,-2.96854e-05,-0.0810844,0.0897801,0.142038,-4.64011e-05,-0.0774198,0.0857671,0.135657,0.08136,0.0629346,0.0311864,0.0239829,0.0333299,0.0258042,-0.145875,-0.112676,
-0.0475902,0.0486847,0.0239397,0.119392,0.0851767,0.168097,0.022895,0.114101,0.08136,0.114573,0.0641084,-0.0295259,-0.04714,0.0772549,0.0352659,-0.0479928,-0.0615756,
0.0360251,-0.00127851,0.018375,0.0922104,0.0658853,-0.0168393,0.017534,0.0880616,0.0629346,0.0641084,0.0520913,-0.0471973,-0.0338508,0.0352646,0.0297955,-0.0616071,-0.0450988,
-0.0478545,0.0489727,-0.168322,-0.114506,0.03268,0.168703,-0.160658,-0.109268,0.0311864,-0.0295259,-0.0471973,0.194324,0.125342,-0.0230317,-0.042181,-0.0274925,-0.0455351,
0.0358023,-0.00103507,-0.129699,-0.0883027,0.0251299,-0.0162988,-0.123832,-0.0843255,0.0239829,-0.04714,-0.0338508,0.125342,0.0990848,-0.0421099,-0.0299681,-0.045501,-0.0325101,
-0.0475843,0.0486959,0.0360788,0.0726428,0.0348901,0.168083,0.034487,0.0694466,0.0333299,0.0772549,0.0352646,-0.0230317,-0.0421099,0.0588844,0.0210688,0.00114858,-0.0235919,
0.0360186,-0.00126519,0.0277662,0.0560765,0.0270105,-0.0168316,0.0265019,0.0535471,0.0258042,0.0352659,0.0297955,-0.042181,-0.0299681,0.0210688,0.018822,-0.0236135,-0.0157344,
-0.0476952,0.0488037,0.10852,-0.0772667,-0.152738,0.168328,0.103657,-0.0737506,-0.145875,-0.0479928,-0.0616071,-0.0274925,-0.045501,0.00114858,-0.0236135,0.188647,0.121254,
0.0359624,-0.00119335,0.0836497,-0.0598519,-0.117979,-0.0166886,0.0798622,-0.0571905,-0.112676,-0.0615756,-0.0450988,-0.0455351,-0.0325101,-0.0235919,-0.0157344,0.121254,0.0961851;
 
  bool check_dmat_gs = dmat_gs.isApprox(dmat_gs_ref,0.0001);
  if(!check_dmat_gs){
      std::cout<<"Result gs"<<std::endl;
      std::cout<<dmat_gs<<std::endl;
      std::cout<<"Ref"<<std::endl;
      std::cout<<dmat_gs_ref<<std::endl;
  }
BOOST_CHECK_EQUAL(check_dmat_gs, 1);
orb.setTDAApprox(false);
orb.BSESingletCoefficients()=Eigen::MatrixXd::Zero(25,2);

orb.BSESingletCoefficients()<<-9.172325499316926E-6,6.465589452839982E-6,
3.7017109343430703E-4,-9.603903429750687E-5,
-1.1309760027198494E-4,-3.573759093655276E-4,
-1.3654831774463808E-5,5.610635944952427E-5,
8.078036128870584E-4,2.3508225792613987E-4,
-4.1389689455982744E-4,3.470240530817982E-4,
0.010935879545252295,-0.0029505107898146012,
-0.003337063524453458,-0.010843128041228025,
-6.022396289068329E-4,0.0017252963787394244,
0.012406560343731091,0.0036243803634561714,
-0.0534328659114822,0.11790152418882739,
-0.014341255912595252,0.002244216527436879,
0.006333920883336163,0.022446822466352594,
-0.013418752450867943,-0.005038670208008163,
-0.012170499758537307,-0.006597144566176471,
-0.2818369874026935,-0.9534596388889569,
-0.020055422721015168,0.019417142136606193,
-0.019391880882433613,-0.009364499588916334,
0.008461551487656474,0.022977612625792006,
-0.0036159338191032687,0.01947317587720039,
-0.9569542694576304,0.2745425547752314,
-0.025652859518621507,-0.019427980787681504,
0.02011572854778333,-0.022825383620618405,
0.016907164519736315,-8.594057567669325E-4,
-0.01566776126491342,-0.0036991931035359783;

orb.BSESingletCoefficientsAR()=Eigen::MatrixXd::Zero(25,2);
orb.BSESingletCoefficientsAR()<<8.507035131871254E-6,-7.771078153320694E-6,
-6.592414632166431E-4,1.8308223920734227E-4,
1.9536776547437147E-4,6.525900973931339E-4,
3.278603510053265E-5,-9.958063857447739E-5,
-0.0011044648385455508,-3.198831712590277E-4,
1.3986989346956674E-4,-1.0389121949776523E-4,
0.011288355421213793,-0.003187756508136955,
-0.0033091549363569992,-0.011220902898107834,
-5.812495659517037E-4,0.0016919649089329493,
-7.785385054188999E-4,-2.135292962729838E-4,
-0.0014926684696797512,0.0032197968277020163,
-0.007005339684942067,0.001062499099375741,
0.0030075906847693325,0.00938345421251267,
-0.0032541108414566823,-0.0036930063078656388,
0.0017512723306478816,9.058993399572328E-4,
-0.007678356616119832,-0.02591115391341041,
-0.009034931850146257,0.01014588913521307,
-0.007764559719931153,-0.004407761436169589,
0.0031656406702596303,0.009697260357617128,
4.730611502982343E-4,-0.0026997355242154015,
-0.026013063963669262,0.007447618201187667,
-0.011436219477735475,-0.00784454627778686,
0.009258965011894391,-0.009898606114539346,
0.006465113614564366,-9.868851256296352E-4,
0.0021031504117920274,5.800186595669497E-4;

  QMState s1=QMState("s1");
  Eigen::MatrixXd dmat_s1=orb.DensityMatrixFull(s1);

  Eigen::MatrixXd dmat_s1_ref=Eigen::MatrixXd::Zero(17,17);
  dmat_s1_ref<<2.0474936814,0.093039309013,0.00027930870559,0.00013412117551,0.0002236273963,-0.45057423895,0.00032447883031,9.6414976839e-05,0.0011453614901,-0.047569979558,0.034564826654,-0.047863976862,0.03605787982,-0.047474712999,0.036553682373,-0.047481413274,0.036747708877,
0.093039309013,0.098607081953,-0.00023279635112,-0.00016344664426,-0.00011627503089,0.27186176359,-0.00025977030654,-0.00017164932468,-0.00051684983536,0.048597963266,-0.00065651947538,0.048987165838,-0.001100443728,0.048577467033,-0.0014975538044,0.048647287156,-0.0015531572202,
0.00027930870559,-0.00023279635112,0.19655470027,0.062483612277,-0.049653026698,-0.0020243015878,0.25644278848,-0.014767477527,0.044645570493,0.019014899921,-0.12447218624,-0.16362165863,-0.13113328777,0.052109906179,0.21355547661,0.092493654522,0.044658112006,
0.00013412117551,-0.00016344664426,0.062483612277,0.13805903626,0.01374586465,-0.00059018682089,-0.022483358425,0.2342384044,-0.082817359094,0.085120139552,0.20997986792,-0.098492150743,-0.060177827507,0.030698425252,-0.18697362472,-0.017306020655,0.037565194415,
0.0002236273963,-0.00011627503089,-0.049653026698,0.01374586465,0.12583059032,-0.003453325581,0.050907693873,-0.086367571801,0.26723896845,0.022964754867,-0.20191414976,0.042207782543,0.027471211902,0.032543541477,0.27453629016,-0.097659837212,-0.094773081646,
-0.45057423895,0.27186176359,-0.0020243015878,-0.00059018682089,-0.003453325581,1.0056879047,-0.0051247721656,-0.0037258816235,-0.016431185902,0.16890751016,0.0040432062202,0.16859040691,-0.020894795359,0.16767752387,-0.024247448135,0.16679382414,-0.027502187342,
0.00032447883031,-0.00025977030654,0.25644278848,-0.022483358425,0.050907693873,-0.0051247721656,0.62174241904,-0.45110587693,0.53643247096,-0.030214912981,-0.75698204974,-0.1575608267,-0.16710662043,0.11609541262,1.0468688747,0.07186019552,-0.11470344006,
9.6414976839e-05,-0.00017164932468,-0.014767477527,0.2342384044,-0.086367571801,-0.0037258816235,-0.45110587693,0.7827128442,-0.58341974054,0.13254479911,0.85226818291,-0.090846092114,0.025514492598,-0.051804518261,-1.166193648,0.010706772158,0.29377112043,
0.0011453614901,-0.00051684983536,0.044645570493,-0.082817359094,0.26723896845,-0.016431185902,0.53643247096,-0.58341974054,0.9908033055,-0.048178244442,-1.1351900819,0.039805445697,0.0015876844895,0.11327220026,1.3415541164,-0.1044896076,-0.18250168934,
-0.047569979558,0.048597963266,0.019014899921,0.085120139552,0.022964754867,0.16890751016,-0.030214912981,0.13254479911,-0.048178244442,0.087000185799,0.14743799655,-0.018593966534,-0.034558271129,0.048726392738,-0.10992334557,-0.0032313074277,-0.014314664106,
0.034564826654,-0.00065651947538,-0.12447218624,0.20997986792,-0.20191414976,0.0040432062202,-0.75698204974,0.85226818291,-1.1351900819,0.14743799655,1.5258086619,-0.038222860143,0.0082651239446,-0.10916739961,-1.6985408041,-0.010179663354,0.13479752897,
-0.047863976862,0.048987165838,-0.16362165863,-0.098492150743,0.042207782543,0.16859040691,-0.1575608267,-0.090846092114,0.039805445697,-0.018593966534,-0.038222860143,0.18556977382,0.11990183188,-0.017133317747,-0.041766372367,-0.035473121585,-0.049165724862,
0.03605787982,-0.001100443728,-0.13113328777,-0.060177827507,0.027471211902,-0.020894795359,-0.16710662043,0.025514492598,0.0015876844895,-0.034558271129,0.0082651239446,0.11990183188,0.12690462965,-0.050224098204,-0.15534424247,-0.044067095492,0.030332887428,
-0.047474712999,0.048577467033,0.052109906179,0.030698425252,0.032543541477,0.16767752387,0.11609541262,-0.051804518261,0.11327220026,0.048726392738,-0.10916739961,-0.017133317747,-0.050224098204,0.064125611198,0.19590547187,0.018181459037,-0.045848940579,
0.036553682373,-0.0014975538044,0.21355547661,-0.18697362472,0.27453629016,-0.024247448135,1.0468688747,-1.166193648,1.3415541164,-0.10992334557,-1.6985408041,-0.041766372367,-0.15534424247,0.19590547187,2.3098850938,-0.053821599569,-0.44099062588,
-0.047481413274,0.048647287156,0.092493654522,-0.017306020655,-0.097659837212,0.16679382414,0.07186019552,0.010706772158,-0.1044896076,-0.0032313074277,-0.010179663354,-0.035473121585,-0.044067095492,0.018181459037,-0.053821599569,0.13469507677,0.10041313642,
0.036747708877,-0.0015531572202,0.044658112006,0.037565194415,-0.094773081646,-0.027502187342,-0.11470344006,0.29377112043,-0.18250168934,-0.014314664106,0.13479752897,-0.049165724862,0.030332887428,-0.045848940579,-0.44099062588,0.10041313642,0.29490774526;


  bool check_dmat_s1 = dmat_s1.isApprox(dmat_s1_ref,0.0001);
  if(!check_dmat_s1){
      std::cout<<"Result s1"<<std::endl;
      std::cout<<dmat_s1<<std::endl;
      std::cout<<"Ref"<<std::endl;
      std::cout<<dmat_s1_ref<<std::endl;
  }
BOOST_CHECK_EQUAL(check_dmat_s1, 1);

  QMState n2s1=QMState("n2s1");
  Eigen::MatrixXd dmat_n2s1=orb.DensityMatrixFull(n2s1);

  Eigen::MatrixXd dmat_n2s1_ref=Eigen::MatrixXd::Zero(17,17);
  dmat_n2s1_ref<<-0.000397846,0.000176625,-0.00881468,0.00292497,-5.88628e-05,0.00567658,-0.0158532,-0.00993011,0.00907284,-0.00270861,0.000263923,-0.000924017,-0.0295645,0.00575722,0.00290312,-0.00234934,0.0177591,
0.000320067,-0.000142077,0.00842665,-0.00278916,0.000112243,-0.00456611,0.0156573,0.00916967,-0.00851408,0.00256481,-0.00076839,0.000922021,0.0281029,-0.00544032,-0.00286575,0.0021369,-0.0175249,
-0.00119257,0.000534394,0.00182468,-0.0294704,-0.0130282,0.0170345,0.0592909,-0.18966,-0.0559797,0.00141623,0.0684471,-0.00279673,-0.0743399,0.0215782,0.173514,-0.0209129,-0.19341,
-0.000825211,0.000364902,-0.046019,0.0405825,-0.0800878,0.0117652,-0.23343,0.204235,-0.397217,0.0365988,0.505462,-0.000632154,-0.0064756,-0.0364718,-0.50527,0.000110369,-0.0117897,
0.000219408,-0.000102567,-0.0415354,0.0585534,-0.0595295,-0.00315313,-0.250057,0.327208,-0.302048,0.0307742,0.386554,0.0016499,0.0528536,-0.0487586,-0.576802,0.0165534,0.141939,
0.00113314,-0.000503003,0.0292259,-0.00966418,0.000269493,-0.0161657,0.053978,0.0321033,-0.0301292,0.00896316,-0.00185594,0.0031865,0.09765,-0.0189548,-0.010399,0.00745341,-0.060809,
-0.00113828,0.000510066,0.00175184,-0.0281414,-0.0124351,0.016259,0.0566329,-0.18108,-0.0534369,0.00135285,0.065327,-0.00266934,-0.0709506,0.0205986,0.165684,-0.0199648,-0.184674,
-0.000787497,0.000348224,-0.0439426,0.0387632,-0.0764923,0.0112275,-0.222946,0.19512,-0.379399,0.0349603,0.482771,-0.00060196,-0.00613095,-0.0348473,-0.482638,0.000112355,-0.0112504,
0.000209371,-9.78784e-05,-0.0396733,0.0559237,-0.0568627,-0.00300889,-0.238837,0.312508,-0.288514,0.0293946,0.369235,0.00157545,0.0504712,-0.046569,-0.550915,0.0158079,0.135546,
-0.000231385,0.000100807,-0.0300844,0.0339378,-0.0581988,0.00329184,-0.176959,0.191403,-0.295432,0.0288963,0.369598,0.000532762,0.0241301,-0.034354,-0.412182,0.0048565,0.0132905,
-0.000342812,0.000150699,-0.0274952,0.0276502,-0.045003,0.00488358,-0.144628,0.143375,-0.223878,0.0210264,0.285828,-4.91689e-05,0.00451078,-0.0238081,-0.317125,0.00268425,0.0192386,
0.00136974,-0.000611345,0.0190123,0.00703061,0.0367438,-0.0195544,0.0426127,0.0803459,0.170366,-0.0118181,-0.221516,0.00286778,0.0749488,-0.0073028,0.03646,0.0170358,0.139817,
0.000891328,-0.000398214,0.0104389,0.00679366,0.02831,-0.0127262,0.025138,0.0571348,0.135819,-0.0104218,-0.170651,0.00174826,0.0435996,-0.00286107,0.0298649,0.0120438,0.116519,
-0.000180458,7.97071e-05,-0.0134737,0.0136744,-0.032556,0.00257215,-0.0826179,0.0810933,-0.166509,0.0162635,0.204982,0.00019225,0.0114336,-0.017132,-0.19887,0.000602948,-0.0215253,
-0.000303473,0.000134402,-0.0146546,0.0119854,-0.0251818,0.00432765,-0.0716987,0.0580983,-0.124224,0.0112615,0.158582,-0.000312438,-0.00530481,-0.010495,-0.152229,-0.000604447,-0.00768288,
-0.000195069,9.22122e-05,0.044308,-0.0611731,0.0540908,0.00280769,0.253367,-0.331024,0.270639,-0.0272218,-0.353694,-0.00143346,-0.0443903,0.0459175,0.567096,-0.0174666,-0.173028,
-0.000314098,0.000143768,0.0300096,-0.0458553,0.0418061,0.00450015,0.187987,-0.260365,0.213736,-0.022355,-0.273329,-0.00156761,-0.0484174,0.0382315,0.439776,-0.0145603,-0.124678;


  bool check_dmat_n2s1 = dmat_n2s1.isApprox(dmat_n2s1_ref,0.0001);
  if(!check_dmat_n2s1){
      std::cout<<"Result n2s1"<<std::endl;
      std::cout<<dmat_n2s1<<std::endl;
      std::cout<<"Ref"<<std::endl;
      std::cout<<dmat_n2s1_ref<<std::endl;
  }

BOOST_CHECK_EQUAL(check_dmat_n2s1, 1);


}


BOOST_AUTO_TEST_CASE(dipole_test) {

  
  std::ofstream xyzfile("molecule.xyz");
  xyzfile << " 5" << endl;
  xyzfile << " methane" << endl;
  xyzfile << " C            .000000     .000000     .000000" << endl;
  xyzfile << " H            .629118     .629118     .629118" << endl;
  xyzfile << " H           -.629118    -.629118     .629118" << endl;
  xyzfile << " H            .629118    -.629118    -.629118" << endl;
  xyzfile << " H           -.629118     .629118    -.629118" << endl;
  xyzfile.close();

  std::ofstream basisfile("3-21G.xml");
  basisfile <<"<basis name=\"3-21G\">" << endl;
  basisfile << "  <element name=\"H\">" << endl;
  basisfile << "    <shell scale=\"1.0\" type=\"S\">" << endl;
  basisfile << "      <constant decay=\"5.447178e+00\">" << endl;
  basisfile << "        <contractions factor=\"1.562850e-01\" type=\"S\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "      <constant decay=\"8.245470e-01\">" << endl;
  basisfile << "        <contractions factor=\"9.046910e-01\" type=\"S\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "    </shell>" << endl;
  basisfile << "    <shell scale=\"1.0\" type=\"S\">" << endl;
  basisfile << "      <constant decay=\"1.831920e-01\">" << endl;
  basisfile << "        <contractions factor=\"1.000000e+00\" type=\"S\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "    </shell>" << endl;
  basisfile << "  </element>" << endl;
  basisfile << "  <element name=\"C\">" << endl;
  basisfile << "    <shell scale=\"1.0\" type=\"S\">" << endl;
  basisfile << "      <constant decay=\"1.722560e+02\">" << endl;
  basisfile << "        <contractions factor=\"6.176690e-02\" type=\"S\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "      <constant decay=\"2.591090e+01\">" << endl;
  basisfile << "        <contractions factor=\"3.587940e-01\" type=\"S\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "      <constant decay=\"5.533350e+00\">" << endl;
  basisfile << "        <contractions factor=\"7.007130e-01\" type=\"S\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "    </shell>" << endl;
  basisfile << "    <shell scale=\"1.0\" type=\"SP\">" << endl;
  basisfile << "      <constant decay=\"3.664980e+00\">" << endl;
  basisfile << "        <contractions factor=\"-3.958970e-01\" type=\"S\"/>" << endl;
  basisfile << "        <contractions factor=\"2.364600e-01\" type=\"P\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "      <constant decay=\"7.705450e-01\">" << endl;
  basisfile << "        <contractions factor=\"1.215840e+00\" type=\"S\"/>" << endl;
  basisfile << "        <contractions factor=\"8.606190e-01\" type=\"P\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "    </shell>" << endl;
  basisfile << "    <shell scale=\"1.0\" type=\"SP\">" << endl;
  basisfile << "      <constant decay=\"1.958570e-01\">" << endl;
  basisfile << "        <contractions factor=\"1.000000e+00\" type=\"S\"/>" << endl;
  basisfile << "        <contractions factor=\"1.000000e+00\" type=\"P\"/>" << endl;
  basisfile << "      </constant>" << endl;
  basisfile << "    </shell>" << endl;
  basisfile << "  </element>" << endl;
  basisfile << "</basis>" << endl;
  basisfile.close();
  
  Orbitals orbitals;
  orbitals.LoadFromXYZ("molecule.xyz");
  BasisSet basis;
  basis.LoadBasisSet("3-21G.xml");
  orbitals.setDFTbasis("3-21G.xml");
  AOBasis aobasis;
  aobasis.AOBasisFill(basis,orbitals.QMAtoms());

  orbitals.setBasisSetSize(17);
  orbitals.setNumberOfLevels(4,13);
 Eigen::MatrixXd& MOs=orbitals.MOCoefficients();
MOs=Eigen::MatrixXd::Zero(17,17);
MOs<<-0.00761992, -4.69664e-13, 8.35009e-15, -1.15214e-14, -0.0156169, -2.23157e-12, 1.52916e-14, 2.10997e-15, 8.21478e-15, 3.18517e-15, 2.89043e-13, -0.00949189, 1.95787e-12, 1.22168e-14, -2.63092e-15, -0.22227, 1.00844,
0.233602, -3.18103e-12, 4.05093e-14, -4.70943e-14, 0.1578, 4.75897e-11, -1.87447e-13, -1.02418e-14, 6.44484e-14, -2.6602e-14, 6.5906e-12, -0.281033, -6.67755e-12, 2.70339e-14, -9.78783e-14, -1.94373, -0.36629,
-1.63678e-13, -0.22745, -0.054851, 0.30351, 3.78688e-11, -0.201627, -0.158318, -0.233561, -0.0509347, -0.650424, 0.452606, -5.88565e-11, 0.453936, -0.165715, -0.619056, 7.0149e-12, 2.395e-14,
-4.51653e-14, -0.216509, 0.296975, -0.108582, 3.79159e-11, -0.199301, 0.283114, -0.0198557, 0.584622, 0.275311, 0.461431, -5.93732e-11, 0.453057, 0.619523, 0.166374, 7.13235e-12, 2.56811e-14,
-9.0903e-14, -0.21966, -0.235919, -0.207249, 3.75979e-11, -0.199736, -0.122681, 0.255585, -0.534902, 0.362837, 0.461224, -5.91028e-11, 0.453245, -0.453298, 0.453695, 7.01644e-12, 2.60987e-14,
0.480866, 1.8992e-11, -2.56795e-13, 4.14571e-13, 2.2709, 4.78615e-10, -2.39153e-12, -2.53852e-13, -2.15605e-13, -2.80359e-13, 7.00137e-12, 0.145171, -1.96136e-11, -2.24876e-13, -2.57294e-14, 4.04176, 0.193617,
-1.64421e-12, -0.182159, -0.0439288, 0.243073, 1.80753e-10, -0.764779, -0.600505, -0.885907, 0.0862014, 1.10077, -0.765985, 6.65828e-11, -0.579266, 0.211468, 0.789976, -1.41532e-11, -1.29659e-13,
-1.64105e-12, -0.173397, 0.23784, -0.0869607, 1.80537e-10, -0.755957, 1.07386, -0.0753135, -0.989408, -0.465933, -0.78092, 6.72256e-11, -0.578145, -0.790571, -0.212309, -1.42443e-11, -1.31306e-13,
-1.63849e-12, -0.17592, -0.188941, -0.165981, 1.79403e-10, -0.757606, -0.465334, 0.969444, 0.905262, -0.61406, -0.78057, 6.69453e-11, -0.578385, 0.578453, -0.578959, -1.40917e-11, -1.31002e-13,
0.129798, -0.274485, 0.00256652, -0.00509635, -0.0118465, 0.141392, -0.000497905, -0.000510338, -0.000526798, -0.00532572, 0.596595, 0.65313, -0.964582, -0.000361559, -0.000717866, -0.195084, 0.0246232,
0.0541331, -0.255228, 0.00238646, -0.0047388, -0.88576, 1.68364, -0.00592888, -0.00607692, -9.5047e-05, -0.000960887, 0.10764, -0.362701, 1.53456, 0.000575205, 0.00114206, -0.793844, -0.035336,
0.129798, 0.0863299, -0.0479412, 0.25617, -0.0118465, -0.0464689, 0.0750316, 0.110468, -0.0436647, -0.558989, -0.203909, 0.65313, 0.320785, 0.235387, 0.878697, -0.195084, 0.0246232,
0.0541331, 0.0802732, -0.0445777, 0.238198, -0.88576, -0.553335, 0.893449, 1.31541, -0.00787816, -0.100855, -0.0367902, -0.362701, -0.510338, -0.374479, -1.39792, -0.793844, -0.035336,
0.129798, 0.0927742, -0.197727, -0.166347, -0.0118465, -0.0473592, 0.0582544, -0.119815, -0.463559, 0.320126, -0.196433, 0.65313, 0.321765, 0.643254, -0.642737, -0.195084, 0.0246232,
0.0541331, 0.0862654, -0.183855, -0.154677, -0.88576, -0.563936, 0.693672, -1.42672, -0.0836372, 0.0577585, -0.0354411, -0.362701, -0.511897, -1.02335, 1.02253, -0.793844, -0.035336,
0.129798, 0.0953806, 0.243102, -0.0847266, -0.0118465, -0.0475639, -0.132788, 0.00985812, 0.507751, 0.244188, -0.196253, 0.65313, 0.322032, -0.87828, -0.235242, -0.195084, 0.0246232,
0.0541331, 0.088689, 0.226046, -0.0787824, -0.88576, -0.566373, -1.58119, 0.117387, 0.0916104, 0.0440574, -0.0354087, -0.362701, -0.512321, 1.39726, 0.374248, -0.793844, -0.035336;

orbitals.setBSEindices(0,16,1);
orbitals.setTDAApprox(true);

MatrixXfd spsi_ref=MatrixXfd::Zero(60,1);
spsi_ref<<-0.000150849,0.00516987,0.0511522,0.00428958,-0.00966668,-0.000155227,1.02978e-08,5.82225e-05,-0.00216177,0.00907102,6.297e-09,-9.84993e-11,0.00159727,
        0.0039042,0.0481196,0.00495382,-0.0106013,0.00025141,-0.000155626,-0.000382828,-0.00322057,0.0124251,1.32177e-05,6.794e-07,
        -0.0153713,0.0200649,-0.067081,-0.0122678,0.0117612,-0.00358901,0.00605007,0.00404793,0.0108884,-0.0151075,-0.000513827,
        -2.64139e-05,-0.0466653,0.0672016,0.021747,-0.0115096,-0.0124868,-0.0115055,0.0187191,0.0124754,0.0149534,0.0112807,-0.00158977,
        -8.17254e-05,-0.00290157,0.0994541,0.984029,0.017835,-0.0401912,-0.000645537,-7.54896e-08,-5.91055e-05,0.00219348,-0.00920484,1.82832e-08,5.56223e-11;

orbitals.BSESingletCoefficients()=spsi_ref;
QMState state_trans=QMState("n2s1");

Eigen::Vector3d res_trans=orbitals.CalcElDipole(state_trans);
Eigen::Vector3d ref_trans=Eigen::Vector3d::Zero();
ref_trans<<  0.118565, 0.0444239,-0.0505149;

bool check_trans=ref_trans.isApprox(res_trans,0.0001);
if(!check_trans){
  std::cout<<"Result transition dipole"<<std::endl;
  std::cout<<res_trans<<std::endl;
  std::cout<<"Ref transition dipole"<<std::endl;
  std::cout<<ref_trans<<std::endl;
}
BOOST_CHECK_EQUAL(check_trans, 1);

QMState state_s1=QMState("s1");
Eigen::Vector3d res_s1=orbitals.CalcElDipole(state_s1);  
Eigen::Vector3d ref_s1=Eigen::Vector3d::Zero();  
ref_s1<<-0.15153501734,-0.42406579479,0.033954362839;
bool check_s1=ref_s1.isApprox(res_s1,0.0001);
if(!check_s1 ){
  std::cout<<"Result s1 dipole"<<std::endl;
  std::cout<<res_s1<<std::endl;
  std::cout<<"Ref s1 dipole"<<std::endl;
  std::cout<<ref_s1<<std::endl;
}
BOOST_CHECK_EQUAL(check_s1, 1);
  
}

BOOST_AUTO_TEST_SUITE_END()
