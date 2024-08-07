#include <gtest/gtest.h>
      
#include <algorithm>
#include <complex>
#include <cstddef>
#include <span>
#include <vector>
#include <array>
#include <numbers>
          
import int3;
import double3;
import double3x3;
import simd_quatd;
  
import units;
import atom;
import pseudo_atom;
import vdwparameters;
import forcefield;
import framework;
import component;
import system;
import simulationbox;
import energy_factor;
import running_energy;
import interactions_intermolecular;
import interactions_framework_molecule;
import interactions_ewald; 
import energy_status;      

static std::array<double3, 300> water_positions = {
    double3( 6.337602942780e+00,      8.754509954240e+00,      7.341184272430e+00),
    double3( 6.301087517290e+00,      9.735069003130e+00,      7.148387552160e+00),
    double3( 7.005507666470e+00,      8.320574597670e+00,      6.736532813830e+00),
    double3( 1.145240808020e+01,     -1.015249483240e+01,     -6.685112139020e+00),
    double3( 1.229189742970e+01,     -1.053876466850e+01,     -6.302943006600e+00),
    double3( 1.094194817920e+01,     -1.086915556050e+01,     -7.160325870170e+00),
    double3( 6.273849631790e+00,     -9.775645901880e+00,     -9.749752395300e+00),
    double3( 6.253118934310e+00,     -8.815147565270e+00,     -1.002726488790e+01),
    double3( 5.371245484130e+00,     -1.018265156100e+01,     -9.889938507640e+00),
    double3(-1.498609889870e+01,      9.189675629980e+00,     -8.977009283260e+00),
    double3(-1.436891486590e+01,      9.148166283650e+00,     -8.191286148930e+00),
    double3(-1.471982063400e+01,      8.496697360540e+00,     -9.646992121740e+00),
    double3(-5.908394927260e-01,     -5.870222524260e+00,     -3.375862522380e-01),
    double3(-1.412152968370e+00,     -5.399976543920e+00,     -1.461118709880e-02),
    double3(-8.435709339040e-01,     -6.760934153470e+00,     -7.154216823160e-01),
    double3(-1.024284998680e+01,      3.738349835280e+00,      1.335007831850e+01),
    double3( 2.510256935630e+01,      3.984597998040e+00,      1.406484122050e+01),
    double3(-1.006698501780e+01,      2.754511838540e+00,      1.338375782040e+01),
    double3(-7.536348696140e-01,      1.031278469350e+01,      6.861209597650e+00),
    double3(-1.331451702290e+00,      9.526826062580e+00,      7.081202184810e+00),
    double3(-4.835409170630e-01,      1.077475668500e+01,      7.705976509810e+00),
    double3(-5.853434589940e+00,     -1.228762585040e+01,     -5.221586124110e+00),
    double3(-5.315458501610e+00,     -1.151971273040e+01,     -5.569280197370e+00),
    double3(-5.745346295060e+00,     -1.307323568320e+01,     -5.830793897420e+00),
    double3( 4.858427137120e+00,     -1.463260325690e+01,     -2.249044124940e+00),
    double3( 4.223042112610e+00,     -1.400238062780e+01,     -2.695256303650e+00),
    double3( 4.433315940120e+00,     -1.500642664620e+01,     -1.424704059330e+00),
    double3(-9.066075825130e+00,      1.285720492320e+00,      1.290793343020e+01),
    double3(-9.110745665810e+00,      1.464294995120e+00,      1.192502155610e+01),
    double3(-8.110208807110e+00,      1.274196640720e+00,      1.320150700650e+01),
    double3(-7.591841246610e-01,      1.077260598800e+01,      3.872148365970e+00),
    double3(-4.661529077090e-01,      1.047019181850e+01,      4.779164559840e+00),
    double3(-1.572439778120e+00,      1.134795874780e+01,      3.959237079290e+00),
    double3(-1.660582291450e+01,     -3.257420529090e+00,     -4.751996911990e+00),
    double3(-1.679227809990e+01,     -3.804624542670e+00,     -3.936029859110e+00),
    double3(-1.653209932910e+01,     -2.293276221290e+00,     -4.497064398000e+00),
    double3(-2.019842097530e+01,     -2.851927240570e+00,     -1.193447052550e+01),
    double3(-2.098102247950e+01,     -2.495247837120e+00,     -1.142426008980e+01),
    double3(-2.007738240300e+01,     -3.821992794190e+00,     -1.172394146410e+01),
    double3(-1.302542580630e+01,      1.422361214180e+01,     -1.290438277450e+01),
    double3(-1.348161611250e+01,      1.341872763400e+01,     -1.328392372580e+01),
    double3(-1.208989258740e+01,      1.427245494050e+01,     -1.325422842230e+01),
    double3( 2.670505982820e+00,     -1.337215801920e+01,     -3.841073403240e+00),
    double3( 2.098182983510e+00,     -1.404619971150e+01,     -4.308100363290e+00),
    double3( 2.188975049870e+00,     -1.249665156440e+01,     -3.800868822890e+00),
    double3(-1.706414220600e+01,     -3.049405032160e+00,     -1.505938557530e+01),
    double3(-1.786184639890e+01,     -2.890508185970e+00,     -1.447764696310e+01),
    double3( 6.589036230470e-01,     -3.021089150870e+00,      1.515706313750e+01),
    double3( 1.863664796530e+01,     -2.690503965680e+00,      4.646140240180e+00),
    double3( 1.827397911110e+01,     -3.528813031260e+00,      5.053214204850e+00),
    double3( 1.963609285570e+01,     -2.713642125350e+00,      4.670109733600e+00),
    double3( 4.659104608170e+00,     -3.934154160370e+00,      8.754976318330e+00),
    double3( 4.978608322720e+00,     -4.656106539280e+00,      8.141215665490e+00),
    double3( 4.327102501740e+00,     -4.342346212750e+00,      9.605360848820e+00),
    double3(-1.680549611310e+01,      7.364829044790e+00,     -6.988904008870e+00),
    double3(-1.648038201260e+01,      7.935615984030e+00,     -7.742895448900e+00),
    double3(-1.768421295550e+01,      6.955965342840e+00,     -7.235251698490e+00),
    double3( 9.204464545180e-01,      3.418254187300e+00,      1.960581229960e+00),
    double3( 1.408118985190e+00,      2.784160112630e+00,      1.360497729690e+00),
    double3( 1.135005203870e+00,      4.358644295150e+00,      1.696704387230e+00),
    double3( 7.340326641100e+00,     -2.012398055600e+00,      9.160016679720e+00),
    double3( 6.901900281920e+00,     -1.951756410300e+00,      1.005673565380e+01),
    double3( 6.729536208420e+00,     -1.634655814070e+00,      8.464138927300e+00),
    double3(-1.120224301980e+01,      1.675663208070e+01,     -1.083244751790e+01),
    double3(-1.176885669480e+01,      1.593314141560e+01,     -1.080395071460e+01),
    double3(-1.168929836930e+01,      1.747955503660e+01,     -1.132250821460e+01),
    double3( 1.179376410760e+01,     -2.564659365450e+00,      7.153381426840e+00),
    double3( 1.124102771650e+01,     -3.308449967530e+00,      7.529224508670e+00),
    double3( 1.276322059220e+01,     -2.796433643610e+00,      7.233598684250e+00),
    double3(-1.724612458270e+01,     -8.400590727130e-02,     -1.315071574450e+00),
    double3(-1.777856056950e+01,      4.390428291260e-02,     -4.783213708580e-01),
    double3(-1.627059137230e+01,     -4.964789846310e-02,     -1.097920745270e+00),
    double3(-6.906780089290e+00,     -9.740367673310e+00,      1.410048088900e+00),
    double3(-7.408256856910e+00,     -1.022588605820e+01,      6.939524601870e-01),
    double3(-6.265905730540e+00,     -1.036790180690e+01,      1.852180385220e+00),
    double3(-1.368318923190e+01,     -6.072688436220e-02,      1.250056059640e-01),
    double3(-1.405662605410e+01,      2.872404123300e-01,      9.849263582740e-01),
    double3(-1.305139257770e+01,      6.118147456460e-01,     -2.603782852140e-01),
    double3( 1.837791989190e+00,     -7.278090292880e+00,     -1.322726353410e+00),
    double3( 1.145739168190e+00,     -6.577556865480e+00,     -1.148611994340e+00),
    double3( 2.504075677910e+00,     -7.281010264960e+00,     -5.770337090130e-01),
    double3(-1.179281871440e+01,     -2.068817951250e+00,     -3.827806789600e+00),
    double3(-1.213807067520e+01,     -2.557885742400e+00,     -3.026798812610e+00),
    double3(-1.110873261460e+01,     -2.631122889000e+00,     -4.292391943290e+00),
    double3(-1.885657084630e+01,     -5.645032276320e+00,     -1.128285523550e+01),
    double3(-1.909681578400e+01,     -6.589281211730e+00,     -1.105774122460e+01),
    double3(-1.791334874640e+01,     -5.465410868890e+00,     -1.100344832740e+01),
    double3(-1.859719317640e+01,     -3.283839673850e+00,     -6.366346021410e+00),
    double3(-1.780832700400e+01,     -3.390552644410e+00,     -5.761116722810e+00),
    double3(-1.844542794810e+01,     -3.804285353250e+00,     -7.206645735940e+00),
    double3(-1.165507322810e+01,     -2.320086344710e+00,     -1.018876401350e+01),
    double3(-1.178238186360e+01,     -1.689690163590e+00,     -9.423000886850e+00),
    double3(-1.166172831110e+01,     -1.807317199710e+00,     -1.104726474960e+01),
    double3( 3.583458471380e+00,      1.250788520580e+01,      1.031954212400e+01),
    double3( 3.078120986530e+00,      1.216666481330e+01,      1.111213449720e+01),
    double3( 4.374540760890e+00,      1.192005037460e+01,      1.015031144570e+01),
    double3(-6.396095565680e+00,      1.502205068990e+01,      1.319029703210e+01),
    double3(-6.019066169890e+00,      1.580744424370e+01,      1.368122044810e+01),
    double3(-6.032885434380e+00,      1.417744057560e+01,      1.358363169300e+01),
    double3( 5.775340626260e+00,      1.070086998800e+01,      1.012081573470e+01),
    double3( 5.148590098630e+00,      1.005071239300e+01,      9.691305690760e+00),
    double3( 6.648695806920e+00,      1.024994986190e+01,      1.030499436600e+01),
    double3(-5.085755988620e+00,     -7.012269399150e+00,      1.805012933190e-01),
    double3(-5.696152121930e+00,     -7.665047595720e+00,      6.291628434060e-01),
    double3(-5.292804472700e+00,     -6.980110410310e+00,     -7.973006931180e-01),
    double3(-7.506855257920e-01,      4.195014663000e+00,      3.980692572570e+00),
    double3(-4.223107380560e-01,      4.812854536060e+00,      4.695146133020e+00),
    double3( 2.954964837980e-02,      3.755651386250e+00,      3.535503866480e+00),
    double3(-1.373349177090e+01,      1.906190306240e-01,     -6.143956456090e+00),
    double3(-1.436958683230e+01,     -4.674278786320e-01,     -6.546892354370e+00),
    double3(-1.324855405580e+01,     -2.382996370830e-01,     -5.381811988830e+00),
    double3( 1.095043612170e+01,     -7.526437387680e+00,     -7.072114935980e+00),
    double3( 1.109847752090e+01,     -8.472352969370e+00,     -6.783449980650e+00),
    double3( 1.141959953390e+01,     -6.908197303390e+00,     -6.441508171960e+00),
    double3(-2.101325969950e+01,     -3.973395016670e+00,     -5.773969078430e+00),
    double3(-2.148023509680e+01,     -4.298775725160e+00,     -6.596198591920e+00),
    double3(-2.016759106810e+01,     -3.507006798750e+00,     -6.033442768950e+00),
    double3(-3.084844365350e+00,     -1.135243772110e+01,     -1.365816717750e+01),
    double3(-3.361819955070e+00,     -1.073176689600e+01,     -1.292464672810e+01),
    double3(-2.100805747120e+00,     -1.152026542260e+01,     -1.359899065820e+01),
    double3(-3.427535955020e+00,      3.410928560390e+00,      4.470022128200e+00),
    double3(-3.552671026240e+00,      2.493997998470e+00,      4.091103194150e+00),
    double3(-2.468855786680e+00,      3.680800844760e+00,      4.380014989820e+00),
    double3(-1.295872324450e+01,     -4.382359919370e+00,     -8.750758280060e+00),
    double3(-1.260933062550e+01,     -3.598379832180e+00,     -9.263885964620e+00),
    double3(-1.323697958900e+01,     -5.100780286990e+00,     -9.388289128020e+00),
    double3(-3.357105559830e+00,      8.473851559810e+00,     -9.756645219910e-01),
    double3(-3.473691279490e+00,      7.799990372530e+00,     -2.460626646450e-01),
    double3(-3.162788180430e+00,      8.003084979950e+00,     -1.836257098850e+00),
    double3(-1.086495550760e+01,      1.157255388510e+01,      3.389571624790e-02),
    double3(-1.011003377660e+01,      1.123413187320e+01,     -5.278548706840e-01),
    double3(-1.135299393030e+01,      1.079869746440e+01,      4.375846286880e-01),
    double3(-7.692670495660e+00,     -1.186553429280e+00,     -4.539633768100e-02),
    double3(-7.300675009830e+00,     -8.683532743000e-01,     -9.085812544060e-01),
    double3(-7.008881138640e+00,     -1.108397295200e+00,      6.800853877510e-01),
    double3( 8.672279395230e+00,     -1.053129190730e+01,     -1.076444307170e+01),
    double3( 9.063124330730e+00,     -9.830848558880e+00,     -1.016726692340e+01),
    double3( 7.717037307380e+00,     -1.068803093480e+01,     -1.051355379160e+01),
    double3( 5.191001542950e+00,     -5.565668325440e+00,      6.618894286680e+00),
    double3( 5.033551408920e+00,     -6.552977481030e+00,      6.598155757420e+00),
    double3( 5.444711442340e+00,     -5.249330244870e+00,      5.704803751920e+00),
    double3(-1.665706680200e+01,     -5.827121736010e+00,     -2.651248595940e+00),
    double3(-1.570448191690e+01,     -6.131191637010e+00,     -2.640134101380e+00),
    double3(-1.726085397900e+01,     -6.623639109270e+00,     -2.682889059970e+00),
    double3(-5.274087675520e+00,      1.211740180100e+01,      1.544293423660e+01),
    double3(-2.408553626670e+01,      1.198461362220e+01,     -1.516484193030e+01),
    double3(-2.249069501670e+01,      1.166445182770e+01,     -1.530838393720e+01),
    double3( 1.109092735840e+01,     -1.582673035710e-01,      6.747159110020e+00),
    double3( 1.144802628530e+01,     -1.050350850830e+00,      7.024047717800e+00),
    double3( 1.157607598510e+01,      1.563357125810e-01,      5.931281820090e+00),
    double3(-1.401780259630e+01,      3.060348991370e+00,     -5.499978875970e+00),
    double3(-1.421438136970e+01,      3.646323908700e+00,     -6.286102391780e+00),
    double3(-1.413220044370e+01,      2.102013953670e+00,     -5.761717463180e+00),
    double3(-1.403244822650e+01,      1.507158443290e+01,      2.399943753990e+00),
    double3(-1.346422546940e+01,      1.428561988690e+01,      2.643629270310e+00),
    double3(-1.474134732390e+01,      1.478354666870e+01,      1.756129870870e+00),
    double3(-9.982085756710e+00,     -6.796599644100e-01,     -1.474493871570e+00),
    double3(-1.079191121770e+01,     -1.243225272810e+00,     -1.637517998270e+00),
    double3(-9.545866118160e+00,     -9.621927533570e-01,     -6.201592473700e-01),
    double3(-6.270598370450e-01,      9.022455181320e+00,     -1.104924285610e+00),
    double3(-1.588695973330e-01,      9.534606005080e+00,     -3.848552381190e-01),
    double3(-1.615138446540e+00,      9.146261312940e+00,     -1.013421809010e+00),
    double3(-9.931534071810e+00,      1.359636366950e+01,     -1.116567712760e+01),
    double3(-9.127646731150e+00,      1.331748800010e+01,     -1.169102796710e+01),
    double3(-9.843207007100e+00,      1.455818109810e+01,     -1.090662971150e+01),
    double3(-1.340147673620e+01,      1.718040555640e+01,      4.704752816910e+00),
    double3(-1.395129354350e+01,      1.645131004850e+01,      4.297173108470e+00),
    double3(-1.255949114810e+01,      1.729707288440e+01,      4.178018534870e+00),
    double3(-1.354000165650e+01,      6.257309716110e+00,     -3.821640911380e+00),
    double3(-1.285659818560e+01,      5.562890703810e+00,     -4.046900608730e+00),
    double3(-1.400498795260e+01,      6.004464906190e+00,     -2.973196896410e+00),
    double3( 1.900102051720e+01,     -1.079540028130e+00,      2.327250176560e+00),
    double3( 1.864666578740e+01,     -1.525809710100e+00,      3.149002022530e+00),
    double3( 1.871038107330e+01,     -1.590968238710e+00,      1.518566122640e+00),
    double3( 9.796645033570e+00,      3.703017541510e-01,      9.217614165710e+00),
    double3( 1.025190095360e+01,      2.759951362520e-01,      8.332262083710e+00),
    double3( 9.701410057180e+00,     -5.305902141270e-01,      9.641080765060e+00),
    double3( 4.805089781740e+00,      3.330328004740e+00,     -2.729328405120e-01),
    double3( 5.786521060430e+00,      3.180739296960e+00,     -1.528667599350e-01),
    double3( 4.655738811630e+00,      4.197863462360e+00,     -7.473551392110e-01),
    double3( 1.122673395840e+00,      7.049612356530e+00,      1.254159455370e+00),
    double3( 1.084167331800e+00,      7.442601553480e+00,      2.172895965000e+00),
    double3( 5.095883196490e-01,      7.557015036880e+00,      6.486272850240e-01),
    double3( 2.026563491990e+01,     -5.920108787600e+00,      8.370789296680e+00),
    double3( 1.944594999460e+01,     -5.388019528860e+00,      8.582915777930e+00),
    double3( 2.052299913410e+01,     -6.467522696830e+00,      9.167094309350e+00),
    double3( 2.003100723540e+01,     -5.806346635450e+00,      5.288835589150e+00),
    double3( 2.004227503220e+01,     -6.351306044450e+00,      6.127222301410e+00),
    double3( 2.094073549140e+01,     -5.423640182690e+00,      5.127806798560e+00),
    double3(-7.653446305330e+00,     -1.165266548860e+01,     -3.202778778270e+00),
    double3(-8.176016349000e+00,     -1.250372546680e+01,     -3.253939925910e+00),
    double3(-6.864971265040e+00,     -1.170833940770e+01,     -3.815320634450e+00),
    double3(-2.517982863320e+01,      1.195948498260e+01,     -1.374696398790e+01),
    double3(-2.590137537380e+01,      1.264978729690e+01,     -1.369355035660e+01),
    double3(-2.504159719490e+01,      1.155129889960e+01,     -1.284459145340e+01),
    double3( 4.405960606180e+00,     -8.362329922990e+00,      5.170391286680e+00),
    double3( 4.815891246970e+00,     -9.273303365180e+00,      5.124737619190e+00),
    double3( 3.852623259680e+00,     -8.204453136700e+00,      4.352532679510e+00),
    double3(-1.614735543410e+01,     -4.905651503660e+00,     -1.173430582650e+01),
    double3(-1.530032095690e+01,     -4.780938198620e+00,     -1.225100611290e+01),
    double3(-1.693051035500e+01,     -4.792478804650e+00,     -1.234574700110e+01),
    double3(-2.613624309230e+00,      5.271162615090e+00,      1.683505803910e+00),
    double3(-2.051565557560e+00,      5.285785191510e+00,      8.565377634580e-01),
    double3(-2.058393709200e+00,      4.969578738140e+00,      2.458596860470e+00),
    double3( 1.361871072010e+01,      6.063778809640e+00,      1.189841471680e+01),
    double3( 1.284369688120e+01,      6.576187286290e+00,      1.226826752880e+01),
    double3( 1.339063460000e+01,      5.727981880200e+00,      1.098451006880e+01),
    double3(-1.008183587130e+01,     -3.690727594280e+00,     -8.004923368880e+00),
    double3(-1.098099654350e+01,     -3.354315275220e+00,     -7.725036182510e+00),
    double3(-1.010186285260e+01,     -3.926742317660e+00,     -8.976466456120e+00),
    double3( 1.451460795510e+01,      6.145547200170e+00,      8.986120641490e+00),
    double3( 1.497041706920e+01,      6.438204827420e+00,      9.826709461750e+00),
    double3( 1.511790299400e+01,      5.532636913510e+00,      8.475850049960e+00),
    double3( 2.373380788300e+00,      1.338551571570e+00,      8.273733509160e-01),
    double3( 3.170817557750e+00,      1.242450854750e+00,      2.316727122100e-01),
    double3( 2.353463360330e+00,      5.829780209870e-01,      1.482134304030e+00),
    double3(-1.309938401560e+01,     -1.668117172270e+01,      6.350633364670e+00),
    double3(-1.287668532550e+01,     -1.711499469440e+01,      7.223675701880e+00),
    double3(-1.321618486830e+01,     -1.738431582920e+01,      5.649244794570e+00),
    double3( 9.570259644870e+00,      1.059655396210e+01,     -7.520597057520e+00),
    double3( 9.503624358550e+00,      9.599225417920e+00,     -7.550522191200e+00),
    double3( 9.633971383400e+00,      1.089795719610e+01,     -6.569231247280e+00),
    double3(-1.670496775760e+00,     -8.962156808090e+00,     -1.259465259020e+01),
    double3(-9.604804140420e-01,     -8.732468984720e+00,     -1.326032538510e+01),
    double3(-1.889309136540e+00,     -8.153652917850e+00,     -1.204835357850e+01),
    double3(-1.030920006130e+01,      1.273981285910e-01,     -1.115071260400e+01),
    double3(-9.852423746820e+00,     -7.787086908550e-04,     -1.027041373610e+01),
    double3(-9.634951922790e+00,      3.899136325340e-01,     -1.184098433160e+01),
    double3(-1.346669042650e+01,      8.089844113980e+00,     -6.387480462420e+00),
    double3(-1.430409736950e+01,      7.656273841340e+00,     -6.054664551610e+00),
    double3(-1.317523776660e+01,      8.793291496650e+00,     -5.739238897250e+00),
    double3( 1.400803180860e+01,     -9.340346835520e+00,     -3.659869321930e+00),
    double3( 1.386384150990e+01,     -8.360082695860e+00,     -3.795114929060e+00),
    double3( 1.414421348750e+01,     -9.526079725220e+00,     -2.686751731910e+00),
    double3(-9.650857229400e+00,      2.258539094580e+00,      1.046487593990e+01),
    double3(-9.200025999250e+00,      1.897266212050e+00,      9.648644902610e+00),
    double3(-1.039566140590e+01,      2.867600385490e+00,      1.019227534820e+01),
    double3( 3.975127664810e+00,      7.276567446300e+00,     -4.094439921890e-01),
    double3( 3.593930552620e+00,      7.219224775890e+00,      5.132697119540e-01),
    double3( 3.238651464100e+00,      7.189828957570e+00,     -1.080323445120e+00),
    double3( 1.251715912490e+01,     -8.580469158700e+00,     -9.643609827200e-01),
    double3( 1.245297606430e+01,     -8.730373634880e+00,      2.225402970870e-02),
    double3( 1.167927324210e+01,     -8.901886197800e+00,     -1.405539330280e+00),
    double3(-2.205829111050e+01,     -3.944489193560e+00,     -8.333856756430e+00),
    double3(-2.118107113690e+01,     -3.652023454770e+00,     -8.714578320630e+00),
    double3(-2.278781533290e+01,     -3.749376034300e+00,     -8.989391091170e+00),
    double3( 1.582277479490e+01,      4.743419163480e+00,      1.194898059910e+01),
    double3( 1.589001069860e+01,      4.005448076190e+00,      1.262045510160e+01),
    double3( 1.486540849820e+01,      5.012702456400e+00,      1.184440391800e+01),
    double3( 2.862718857050e+00,      1.072739190510e+01,      1.706208339130e+00),
    double3( 2.989272559670e+00,      1.137524077960e+01,      2.457391403490e+00),
    double3( 3.721097954450e+00,      1.024381012910e+01,      1.534936651330e+00),
    double3( 2.297072310120e+00,     -7.247046985660e+00,     -4.491427033530e+00),
    double3( 2.388593363830e+00,     -7.891802066980e+00,     -3.732536539560e+00),
    double3( 2.488870201830e+00,     -6.320835316890e+00,     -4.166857297770e+00),
    double3(-1.779512127100e+01,     -9.192965739130e-01,     -8.600345741490e+00),
    double3(-1.794638121910e+01,     -1.587336975140e+00,     -7.871756384990e+00),
    double3(-1.683633082350e+01,     -9.459465500450e-01,     -8.883207291830e+00),
    double3(-1.634461136180e+01,     -4.374768027040e+00,     -7.874122426430e+00),
    double3(-1.587610554320e+01,     -4.459391863230e+00,     -8.753520575360e+00),
    double3(-1.570591792880e+01,     -4.586733120970e+00,     -7.134432290210e+00),
    double3(-2.763178888840e+00,      5.913660155950e+00,      7.405045713060e+00),
    double3(-3.322680655430e+00,      6.671483084480e+00,      7.069393836570e+00),
    double3(-3.317391622360e+00,      5.082231710430e+00,      7.444731806090e+00),
    double3(-9.534710535070e+00,     -3.703278567850e+00,     -4.762074633320e+00),
    double3(-9.838655693610e+00,     -4.434613478610e+00,     -5.372620773180e+00),
    double3(-9.428357751840e+00,     -2.856541148630e+00,     -5.283346950250e+00),
    double3( 1.133479162610e+01,     -6.250264483170e+00,     -3.005315160970e+00),
    double3( 1.177422897630e+01,     -5.418743016550e+00,     -3.345119318980e+00),
    double3( 1.199354667960e+01,     -6.781556544090e+00,     -2.472614001340e+00),
    double3( 1.222921112950e+01,      1.125281856700e+01,     -5.739030258150e+00),
    double3( 1.310730462340e+01,      1.079421856210e+01,     -5.875550774770e+00),
    double3( 1.236227050810e+01,      1.224345902340e+01,     -5.769471806180e+00),
    double3( 1.397724757750e+01,     -3.426277720020e+00,      8.648195815460e+00),
    double3( 1.447341729480e+01,     -2.687169891130e+00,      8.192635469480e+00),
    double3( 1.375004116690e+01,     -3.149269085990e+00,      9.581814294120e+00),
    double3(-4.688760502520e-01,      4.931529295250e+00,      1.419702979270e+01),
    double3(-8.672486061500e-01,      4.155254463420e+00,      1.370846383790e+01),
    double3(-8.072366498060e-01,      4.942335391600e+00,      1.513798427250e+01),
    double3( 1.684924385380e+01,     -1.466617313430e+01,      1.524137563240e+01),
    double3( 1.661841607530e+01,     -1.563909154710e+01,      1.522919783670e+01),
    double3( 1.602997533790e+01,     -1.412677305020e+01,      1.504683348250e+01),
    double3(-2.359247526450e+01,     -4.151242291020e+00,     -1.318653230950e+01),
    double3(-2.418044553690e+01,     -4.748206572990e+00,     -1.373235701100e+01),
    double3(-2.268894423450e+01,     -4.569215828410e+00,     -1.309203469110e+01),
    double3( 1.876020593740e+01,     -8.086352565020e+00,      2.575332524090e+00),
    double3(-1.671135882380e+01,     -8.786452710470e+00,      2.095107592770e+00),
    double3( 1.898594209040e+01,     -8.107941642350e+00,      3.549281753250e+00),
    double3(-1.377944948820e+01,     -1.492422671360e+01,     -1.169887100140e+01),
    double3(-1.330935001390e+01,     -1.432697567820e+01,     -1.104902664700e+01),
    double3(-1.336710757620e+01,     -1.583489159350e+01,     -1.167311009610e+01),
    double3( 2.044471948170e+01,     -9.468568484680e+00,      8.080822044880e+00),
    double3( 2.038576978720e+01,     -1.021619885520e+01,      8.742315629630e+00),
    double3( 2.139217085560e+01,     -9.155460009200e+00,      8.015255463370e+00),
    double3(-1.805140997350e+01,      1.040954968840e+01,     -8.111676770390e+00),
    double3(-1.878379437300e+01,      1.051944541120e+01,     -8.783641071110e+00),
    double3(-1.727098959280e+01,      1.097740517320e+01,     -8.373371588330e+00),
    double3(-1.504436265480e+01,     -1.206977706960e+01,     -2.999451990630e-01),
    double3(-1.418405504470e+01,     -1.169178944360e+01,     -6.419922289280e-01),
    double3(-1.485438401340e+01,     -1.287267728520e+01,      2.650852068730e-01),
};

TEST(nist_water_4, Test_NIST_water_100_monoclinic)
{
  ForceField forceField = ForceField(
      {
          PseudoAtom("O", 15.9996, -0.84760, 0.0, 8, true),
          PseudoAtom("H",  1.0008,  0.42380, 0.0, 1, true)
      },
      {
          VDWParameters(78.19743111, 3.16555789),
          VDWParameters(0.0, 1.0)
      },
      ForceField::MixingRule::Lorentz_Berthelot, 10.0, false, false, true);

  SimulationBox box =  SimulationBox(36.0, 36.0, 36.0, 90.0 * (std::numbers::pi / 180.0), 60.0 * (std::numbers::pi / 180.0),
                                     90.0 * (std::numbers::pi / 180.0));

  forceField.automaticEwald = false;
  forceField.EwaldAlpha = 0.2850;
  forceField.numberOfWaveVectors = int3(7, 7, 7);

  const double kvecx = forceField.numberOfWaveVectors.x * 2.0 * std::numbers::pi / box.cell.ax;
  const double kvecy = forceField.numberOfWaveVectors.y * 2.0 * std::numbers::pi / box.cell.by;
  const double kvecz = forceField.numberOfWaveVectors.z * 2.0 * std::numbers::pi / box.cell.cz;
  forceField.reciprocalCutOffSquared = std::max({kvecx*kvecx, kvecy*kvecy, kvecz*kvecz}) * 1.00001;

  Component c = Component(0, forceField, "H2O", 304.1282, 7377300.0, 0.22394,
                          {// double3 position, double charge, double lambda, uint32_t moleculeId, uint16_t type,
                           // uint8_t componentId, uint8_t groupId
                           Atom(double3( 0.00000, -0.06461,  0.00000), -0.84760, 1.0, 0, 0, 0, 0),
                           Atom(double3( 0.81649,  0.51275,  0.00000),  0.42380, 1.0, 0, 1, 0, 0),
                           Atom(double3(-0.81649,  0.51275,  0.00000),  0.42380, 1.0, 0, 1, 0, 0)
                          },
                          5, 21);

  System system = System(0, forceField, box, 300.0, 1e4, 1.0, {}, {c}, {100}, 5);

  std::span<Atom> moleculeAtomPositions = system.spanOfMoleculeAtoms();

  for(size_t i = 0; i < water_positions.size(); ++i)
  {
    moleculeAtomPositions[i].position = water_positions[i];
  }

  RunningEnergy energy = system.computeTotalEnergies();

  EXPECT_NEAR(energy.moleculeMoleculeVDW * Units::EnergyToKelvin, 2.50251e+04, 0.00001e+04);
  EXPECT_NEAR(energy.moleculeMoleculeCharge * Units::EnergyToKelvin, -1.71462e+05, 0.00001e+05);
  EXPECT_NEAR(energy.ewald_fourier * Units::EnergyToKelvin, 2.23372e+04, 0.00001e+04);
  EXPECT_NEAR(energy.ewald_self * Units::EnergyToKelvin, -2.89549e+06, 0.00001e+06);
  EXPECT_NEAR(energy.ewald_exclusion * Units::EnergyToKelvin, 2.85884e+06, 0.00001e+06);
}

