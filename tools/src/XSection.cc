// #include "../interface/XSection.h"
#include "tools/interface/XSection.h"
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

namespace {
  map < float, map < float, float > > gqs()
  {
    static map < float, map < float, float > >  gq_xsec;
    static bool fill=true;
    if (fill)
    {
      // Eder Izaguirre, oops wrong order!
      gq_xsec[100.][100.]=22400.;
      gq_xsec[100.][150.]=10900.;
      gq_xsec[100.][200.]= 5630.;
      gq_xsec[100.][250.]= 3110.;
      gq_xsec[100.][300.]= 1810.;
      gq_xsec[100.][350.]= 1100.;
      gq_xsec[100.][400.]=  694.;
      gq_xsec[100.][450.]=  451.;
      gq_xsec[100.][500.]=  301.;
      gq_xsec[100.][550.]=  205.;
      gq_xsec[100.][600.]=  142.;
      gq_xsec[100.][650.]=  100.;
      gq_xsec[100.][700.]=   71.9;
      gq_xsec[100.][750.]=   52.1;
      gq_xsec[100.][800.]=   38.2;
      gq_xsec[100.][850.]=   28.3;
      gq_xsec[100.][900.]=   21.1;

      gq_xsec[150.][100.]=   7480.;
      gq_xsec[150.][150.]=   3770.;
      gq_xsec[150.][200.]=   2210.;
      gq_xsec[150.][250.]=   1330.;
      gq_xsec[150.][300.]=    825.;
      gq_xsec[150.][350.]=    529.;
      gq_xsec[150.][400.]=    348.;
      gq_xsec[150.][450.]=    234.;
      gq_xsec[150.][500.]=    161.;
      gq_xsec[150.][550.]=    112.;
      gq_xsec[150.][600.]=     79.6;
      gq_xsec[150.][650.]=     57.3;
      gq_xsec[150.][700.]=     41.7;
      gq_xsec[150.][750.]=     30.7;
      gq_xsec[150.][800.]=     22.8;
      gq_xsec[150.][850.]=     17.1;
      gq_xsec[150.][900.]=     12.9;
                   
      gq_xsec[200.][100.]=    3200.;
      gq_xsec[200.][150.]=    1650.;
      gq_xsec[200.][200.]=     976.;
      gq_xsec[200.][250.]=     633.;
      gq_xsec[200.][300.]=     414.;
      gq_xsec[200.][350.]=     277.;
      gq_xsec[200.][400.]=     189.;
      gq_xsec[200.][450.]=     131.;
      gq_xsec[200.][500.]=      92.3;
      gq_xsec[200.][550.]=      65.9;
      gq_xsec[200.][600.]=      47.6;
      gq_xsec[200.][650.]=      34.8;
      gq_xsec[200.][700.]=      25.7;
      gq_xsec[200.][750.]=      19.2;
      gq_xsec[200.][800.]=      14.4;
      gq_xsec[200.][850.]=      10.9;
      gq_xsec[200.][900.]=       8.32;
                   
      gq_xsec[250.][100.]=    1520.;
      gq_xsec[250.][150.]=     841.;
      gq_xsec[250.][200.]=     498.;
      gq_xsec[250.][250.]=     323.;
      gq_xsec[250.][300.]=     223.;
      gq_xsec[250.][350.]=     154.;
      gq_xsec[250.][400.]=     108.;
      gq_xsec[250.][450.]=      77.;
      gq_xsec[250.][500.]=      55.4;
      gq_xsec[250.][550.]=      40.3;
      gq_xsec[250.][600.]=      29.7;
      gq_xsec[250.][650.]=      22.0;
      gq_xsec[250.][700.]=      16.5;
      gq_xsec[250.][750.]=      12.4;
      gq_xsec[250.][800.]=      9.44;
      gq_xsec[250.][850.]=      7.22;
      gq_xsec[250.][900.]=      5.55;
                   
      gq_xsec[300.][100.]=    817.;
      gq_xsec[300.][150.]=    455.;
      gq_xsec[300.][200.]=    281.;
      gq_xsec[300.][250.]=    181.;
      gq_xsec[300.][300.]=    125.;
      gq_xsec[300.][350.]=     89.8;
      gq_xsec[300.][400.]=     64.6;
      gq_xsec[300.][450.]=     46.9;
      gq_xsec[300.][500.]=     34.5;
      gq_xsec[300.][550.]=     25.5;
      gq_xsec[300.][600.]=     19.;
      gq_xsec[300.][650.]=     14.3;
      gq_xsec[300.][700.]=     10.9;
      gq_xsec[300.][750.]=     8.28;
      gq_xsec[300.][800.]=     6.35;
      gq_xsec[300.][850.]=     4.90;
      gq_xsec[300.][900.]=     3.80;
                   
      gq_xsec[350.][100.]=     461.;
      gq_xsec[350.][150.]=     271.;
      gq_xsec[350.][200.]=     166.;
      gq_xsec[350.][250.]=     110.;
      gq_xsec[350.][300.]=      74.8;
      gq_xsec[350.][350.]=      53.6;
      gq_xsec[350.][400.]=      39.9;
      gq_xsec[350.][450.]=      29.5;
      gq_xsec[350.][500.]=      22.0;
      gq_xsec[350.][550.]=      16.6;
      gq_xsec[350.][600.]=      12.5;
      gq_xsec[350.][650.]=       9.54;
      gq_xsec[350.][700.]=       7.30;
      gq_xsec[350.][750.]=       5.62;
      gq_xsec[350.][800.]=       4.35;
      gq_xsec[350.][850.]=       3.39;
      gq_xsec[350.][900.]=       2.64;
                   
      gq_xsec[400.][100.]=    272.;
      gq_xsec[400.][150.]=    166.;
      gq_xsec[400.][200.]=    105.;
      gq_xsec[400.][250.]=     68.5;
      gq_xsec[400.][300.]=     47.6;
      gq_xsec[400.][350.]=     33.9;
      gq_xsec[400.][400.]=     25.0;
      gq_xsec[400.][450.]=     19.0;
      gq_xsec[400.][500.]=     14.4;
      gq_xsec[400.][550.]=     11.0;
      gq_xsec[400.][600.]=      8.4;
      gq_xsec[400.][650.]=      6.46;
      gq_xsec[400.][700.]=      5.0;
      gq_xsec[400.][750.]=     3.88;
      gq_xsec[400.][800.]=     3.03;
      gq_xsec[400.][850.]=     2.38;
      gq_xsec[400.][900.]=     1.87;
                   
      gq_xsec[450.][100.]=     167.;
      gq_xsec[450.][150.]=     104.;
      gq_xsec[450.][200.]=      68.2;
      gq_xsec[450.][250.]=      45.6;
      gq_xsec[450.][300.]=      31.1;
      gq_xsec[450.][350.]=      22.4;
      gq_xsec[450.][400.]=      16.4;
      gq_xsec[450.][450.]=      12.4;
      gq_xsec[450.][500.]=      9.60;
      gq_xsec[450.][550.]=      7.40;
      gq_xsec[450.][600.]=      5.72;
      gq_xsec[450.][650.]=      4.45;
      gq_xsec[450.][700.]=      3.47;
      gq_xsec[450.][750.]=      2.72;
      gq_xsec[450.][800.]=      2.14;
      gq_xsec[450.][850.]=      1.69;
      gq_xsec[450.][900.]=      1.34;
                   
      gq_xsec[500.][100.]=    105.;
      gq_xsec[500.][150.]=     67.2;
      gq_xsec[500.][200.]=     44.9;
      gq_xsec[500.][250.]=     30.8;
      gq_xsec[500.][300.]=     21.4;
      gq_xsec[500.][350.]=     15.1;
      gq_xsec[500.][400.]=     11.2;
      gq_xsec[500.][450.]=      8.39;
      gq_xsec[500.][500.]=      6.45;
      gq_xsec[500.][550.]=      5.07;
      gq_xsec[500.][600.]=      3.96;
      gq_xsec[500.][650.]=      3.10;
      gq_xsec[500.][700.]=      2.44;
      gq_xsec[500.][750.]=      1.93;
      gq_xsec[500.][800.]=     1.53;
      gq_xsec[500.][850.]=     1.21;
      gq_xsec[500.][900.]=     0.965;
                   
      gq_xsec[550.][100.]= 68.;
      gq_xsec[550.][150.]= 44.4;
      gq_xsec[550.][200.]= 30.1;
      gq_xsec[550.][250.]= 21.0;
      gq_xsec[550.][300.]= 14.9;
      gq_xsec[550.][350.]= 10.7;
      gq_xsec[550.][400.]=  7.73;
      gq_xsec[550.][450.]=  5.85;
      gq_xsec[550.][500.]=  4.47;
      gq_xsec[550.][550.]=  3.49;
      gq_xsec[550.][600.]=  2.77;
      gq_xsec[550.][650.]=  2.19;
      gq_xsec[550.][700.]=  1.74;
      gq_xsec[550.][750.]=  1.38;
      gq_xsec[550.][800.]=  1.1;
      gq_xsec[550.][850.]=  0.878;
      gq_xsec[550.][900.]=  0.703;
                   
      gq_xsec[600.][100.]=  45.8;
      gq_xsec[600.][150.]=  30.0;
      gq_xsec[600.][200.]=  20.6;
      gq_xsec[600.][250.]=  14.6;
      gq_xsec[600.][300.]=  10.5;
      gq_xsec[600.][350.]=   7.63;
      gq_xsec[600.][400.]=   5.60;
      gq_xsec[600.][450.]=   4.13;
      gq_xsec[600.][500.]=   3.18;
      gq_xsec[600.][550.]=   2.47;
      gq_xsec[600.][600.]=   1.95;
      gq_xsec[600.][650.]=   1.56;
      gq_xsec[600.][700.]=   1.25;
      gq_xsec[600.][750.]=   0.997;
      gq_xsec[600.][800.]=   0.799;
      gq_xsec[600.][850.]=   0.642;
      gq_xsec[600.][900.]=   0.516;
                   
      gq_xsec[650.][100.]= 31.1;
      gq_xsec[650.][150.]= 20.6;
      gq_xsec[650.][200.]= 14.3;
      gq_xsec[650.][250.]= 10.2;
      gq_xsec[650.][300.]=  7.44;
      gq_xsec[650.][350.]=  5.48;
      gq_xsec[650.][400.]=  4.07;
      gq_xsec[650.][450.]=  3.05;
      gq_xsec[650.][500.]=  2.29;
      gq_xsec[650.][550.]=  1.79;
      gq_xsec[650.][600.]=  1.40;
      gq_xsec[650.][650.]=  1.12;
      gq_xsec[650.][700.]=  0.904;
      gq_xsec[650.][750.]=  0.726;
      gq_xsec[650.][800.]=  0.585;
      gq_xsec[650.][850.]=  0.472;
      gq_xsec[650.][900.]=  0.382;
                   
      gq_xsec[700.][100.]=  21.6;
      gq_xsec[700.][150.]=  14.4;
      gq_xsec[700.][200.]=  10.1;
      gq_xsec[700.][250.]=  7.29;
      gq_xsec[700.][300.]=  5.35;
      gq_xsec[700.][350.]=  3.97;
      gq_xsec[700.][400.]=  2.98;
      gq_xsec[700.][450.]=  2.25;
      gq_xsec[700.][500.]=  1.71;
      gq_xsec[700.][550.]=  1.30;
      gq_xsec[700.][600.]=  1.03;
      gq_xsec[700.][650.]=  0.816;
      gq_xsec[700.][700.]=  0.655;
      gq_xsec[700.][750.]=  0.533;
      gq_xsec[700.][800.]=  0.431;
      gq_xsec[700.][850.]=  0.350;
      gq_xsec[700.][900.]=  0.284;
                   
      gq_xsec[750.][100.]= 15.2;
      gq_xsec[750.][150.]= 10.2;
      gq_xsec[750.][200.]=  7.23;
      gq_xsec[750.][250.]=  5.25;
      gq_xsec[750.][300.]=  3.88;
      gq_xsec[750.][350.]=  2.90;
      gq_xsec[750.][400.]=  2.19;
      gq_xsec[750.][450.]=  1.67;
      gq_xsec[750.][500.]=  1.28;
      gq_xsec[750.][550.]=  0.985;
      gq_xsec[750.][600.]=  0.759;
      gq_xsec[750.][650.]=  0.605;
      gq_xsec[750.][700.]=  0.484;
      gq_xsec[750.][750.]=  0.391;
      gq_xsec[750.][800.]=  0.32;
      gq_xsec[750.][850.]=  0.261;
      gq_xsec[750.][900.]=  0.212;
                   
      gq_xsec[800.][100.]=  10.9;
      gq_xsec[800.][150.]=   7.35;
      gq_xsec[800.][200.]=   5.23;
      gq_xsec[800.][250.]=   3.82;
      gq_xsec[800.][300.]=   2.84;
      gq_xsec[800.][350.]=   2.14;
      gq_xsec[800.][400.]=   1.63;
      gq_xsec[800.][450.]=   1.25;
      gq_xsec[800.][500.]=   0.961;
      gq_xsec[800.][550.]=   0.745;
      gq_xsec[800.][600.]=   0.580;
      gq_xsec[800.][650.]=   0.452;
      gq_xsec[800.][700.]=   0.363;
      gq_xsec[800.][750.]=   0.292;
      gq_xsec[800.][800.]=   0.237;
      gq_xsec[800.][850.]=   0.195;
      gq_xsec[800.][900.]=   0.160;
                   
      gq_xsec[850.][100.]= 7.85;
      gq_xsec[850.][150.]= 5.36;
      gq_xsec[850.][200.]= 3.82;
      gq_xsec[850.][250.]= 2.81;
      gq_xsec[850.][300.]= 2.10;
      gq_xsec[850.][350.]= 1.59;
      gq_xsec[850.][400.]= 1.22;
      gq_xsec[850.][450.]= 0.937;
      gq_xsec[850.][500.]= 0.726;
      gq_xsec[850.][550.]= 0.565;
      gq_xsec[850.][600.]= 0.443;
      gq_xsec[850.][650.]= 0.348;
      gq_xsec[850.][700.]= 0.273;
      gq_xsec[850.][750.]= 0.221;
      gq_xsec[850.][800.]= 0.179;
      gq_xsec[850.][850.]= 0.146;
      gq_xsec[850.][900.]= 0.121;
                   
      gq_xsec[900.][100.]=  5.73;
      gq_xsec[900.][150.]=  3.93;
      gq_xsec[900.][200.]=  2.83;
      gq_xsec[900.][250.]=  2.08;
      gq_xsec[900.][300.]=  1.56;
      gq_xsec[900.][350.]=  1.19;
      gq_xsec[900.][400.]=  0.915;
      gq_xsec[900.][450.]=  0.708;
      gq_xsec[900.][500.]=  0.551;
      gq_xsec[900.][550.]=  0.432;
      gq_xsec[900.][600.]=  0.339;
      gq_xsec[900.][650.]=  0.268;
      gq_xsec[900.][700.]=  0.212;
      gq_xsec[900.][750.]=  0.168;
      gq_xsec[900.][800.]=  0.136;
      gq_xsec[900.][850.]=  0.111;
      gq_xsec[900.][900.]=  0.091;
    fill=false;
    }

    return gq_xsec;
  }

  map < float, float > ggs()
  {
    static map < float, float >  gg_xsec;
    static bool fill=true;
    if (fill)
    {
      /* old NLO values
      gg_xsec[50.]=484000.;
      gg_xsec[75.]=81000.;
      gg_xsec[100.]=21200.;
      gg_xsec[125.]= 7170.;
      gg_xsec[150.]= 2860.;
      gg_xsec[175.]= 1280.;
      gg_xsec[200.]=  625.;
      gg_xsec[225.]=  326.;
      gg_xsec[250.]=  180.;
      gg_xsec[275.]=  104.;
      gg_xsec[300.]=   62.1;
      gg_xsec[325.]=   38.3;
      gg_xsec[350.]=   24.2;
      gg_xsec[375.]=   15.7;
      gg_xsec[400.]=   10.4;
      gg_xsec[425.]=    6.97;
      gg_xsec[450.]=    4.76;
      gg_xsec[475.]=    3.3;
      gg_xsec[500.]=    2.31;
      gg_xsec[525.]=    1.64;
      gg_xsec[550.]=    1.17;
      gg_xsec[575.]=    0.847;
      gg_xsec[600.]=    0.617;
      gg_xsec[625.]=    0.453;
      gg_xsec[650.]=    0.335;
      gg_xsec[675.]=    0.249;
      gg_xsec[700.]=    0.186;
      gg_xsec[725.]=    0.140;
      gg_xsec[750.]=    0.106;
      gg_xsec[775.]=    0.08;
      gg_xsec[800.]=    0.061;
      gg_xsec[825.]=    0.046;
      gg_xsec[850.]=    0.0358;
      gg_xsec[875.]=    0.0275;
      gg_xsec[900.]=    0.0212;
      gg_xsec[925.]=    0.0164;
      gg_xsec[950.]=    0.0128;
      gg_xsec[1000.]=   0.00772;
      gg_xsec[1050.]=   0.00471;
      gg_xsec[1100.]=   0.0029;
      gg_xsec[1150.]=   0.00179;
      gg_xsec[1200.]=   0.00111;
      */
      gg_xsec[50.]=484000.;
      gg_xsec[75.]=81000.;
      gg_xsec[100.]=21200.;
      gg_xsec[125.]= 7170.;
      gg_xsec[150.]= 2860.;
      gg_xsec[175.]= 1280.;
      gg_xsec[200.]=  625.;
      gg_xsec[225.]=  326.;
      gg_xsec[250.]=  180.;
      gg_xsec[275.]=  104.;
      gg_xsec[300.]=   62.1;
      gg_xsec[325.]=   38.3;
      gg_xsec[350.]=   24.3;
      gg_xsec[375.]=   15.7;
      gg_xsec[400.]=   10.4;
      gg_xsec[425.]=    7.01;
      gg_xsec[450.]=    4.80;
      gg_xsec[475.]=    3.33;
      gg_xsec[500.]=    2.34;
      gg_xsec[525.]=    1.66;
      gg_xsec[550.]=    1.20;
      gg_xsec[575.]=    0.867;
      gg_xsec[600.]=    0.634;
      gg_xsec[625.]=    0.467;
      gg_xsec[650.]=    0.346;
      gg_xsec[675.]=    0.259;
      gg_xsec[700.]=    0.194;
      gg_xsec[725.]=    0.147;
      gg_xsec[750.]=    0.111;
      gg_xsec[775.]=    0.085;
      gg_xsec[800.]=    0.065;
      gg_xsec[825.]=    0.050;
      gg_xsec[850.]=    0.03870;
      gg_xsec[875.]=    0.02990;
      gg_xsec[900.]=    0.02330;
      gg_xsec[925.]=    0.01810;
      gg_xsec[950.]=    0.01420;
      gg_xsec[975.]=    0.01110;
      gg_xsec[1000.]=   0.00870;
      gg_xsec[1025.]=   0.00684;
      gg_xsec[1050.]=   0.00539;
      gg_xsec[1075.]=   0.00425;
      gg_xsec[1100.]=   0.00336;
      gg_xsec[1125.]=   0.00266;
      gg_xsec[1150.]=   0.00211;
      gg_xsec[1175.]=   0.00167;
      gg_xsec[1200.]=   0.00133;
      gg_xsec[1225.]=   0.00106;
      gg_xsec[1250.]=   0.00084;
      gg_xsec[1275.]=   0.00067;
      gg_xsec[1300.]=   0.00053;
      gg_xsec[1325.]=   0.00042;
      gg_xsec[1350.]=   0.00034;
      fill=false;
    }
    return gg_xsec;
  }

  map < float, float > bbs()
  {
    static map < float, float >  sq_xsec;
    static bool fill=true;
    if (fill)
    {
      float f=.125;
      // Eder Izaguirre
      sq_xsec[100.]=3930.*f;
      sq_xsec[150.]=547.*f;
      sq_xsec[200.]=123.*f;
      sq_xsec[250.]=36.8*f;
      sq_xsec[300.]=13.3*f;
      sq_xsec[350.]= 5.48*f;
      sq_xsec[400.]= 2.49*f;
      sq_xsec[450.]=1.22*f;
      sq_xsec[500.]=0.630*f;
      sq_xsec[550.]=0.342*f;
      sq_xsec[600.]=0.193*f;
      sq_xsec[650.]=0.112*f;
      sq_xsec[700.]=0.0666*f;
      sq_xsec[750.]=0.0404*f;
      sq_xsec[800.]=0.0249*f;
      sq_xsec[850.]=0.0155*f;
      sq_xsec[900.]=0.00981*f;
      sq_xsec[950.]=0.00624*f;
      sq_xsec[1000.]=0.00399*f;
      sq_xsec[1050.]=0.00257*f;
      sq_xsec[1100.]=0.00166*f;
      sq_xsec[1150.]=0.00108*f;
      sq_xsec[1200.]=0.000699*f;
      fill=false;
    }
    return sq_xsec;
  } 

  map < float, float > qqs()
  {
    static map < float, float >  sq_xsec;
    static bool fill=true;
    if (fill)
    {
      // Eder Izaguirre
      sq_xsec[100.]=3930.;
      sq_xsec[150.]=547.;
      sq_xsec[200.]=123.;
      sq_xsec[250.]=36.8;
      sq_xsec[300.]=13.3;
      sq_xsec[350.]= 5.48;
      sq_xsec[400.]= 2.49;
      sq_xsec[450.]=1.22;
      sq_xsec[500.]=0.630;
      sq_xsec[550.]=0.342;
      sq_xsec[600.]=0.193;
      sq_xsec[650.]=0.112;
      sq_xsec[700.]=0.0666;
      sq_xsec[750.]=0.0404;
      sq_xsec[800.]=0.0249;
      sq_xsec[850.]=0.0155;
      sq_xsec[900.]=0.00981;
      sq_xsec[950.]=0.00624;
      sq_xsec[1000.]=0.00399;
      sq_xsec[1050.]=0.00257;
      sq_xsec[1100.]=0.00166;
      sq_xsec[1150.]=0.00108;
      sq_xsec[1200.]=0.000699;
      fill=false;
    }
    return sq_xsec;
  } 
  
  map < float, float > tts()
  {
    static map < float, float >  st_xsec;
    static bool fill=true;
    if (fill)
    {
      // Maria
      st_xsec[150.]=53.100000;
      st_xsec[160.]=38.200000;
      st_xsec[170.]=27.900000;
      st_xsec[180.]=20.700000;
      st_xsec[190.]=15.600000;
      st_xsec[200.]=11.800000;
      st_xsec[210.]=9.100000;
      st_xsec[220.]=7.060000;
      st_xsec[230.]=5.530000;
      st_xsec[240.]=4.360000;
      st_xsec[250.]=3.470000;
      st_xsec[260.]=2.780000;
      st_xsec[270.]=2.240000;
      st_xsec[280.]=1.810000;
      st_xsec[290.]=1.470000;
      st_xsec[300.]=1.200000;
      st_xsec[310.]=0.995000;
      st_xsec[320.]=0.823000;
      st_xsec[330.]=0.683000;
      st_xsec[340.]=0.566000;
      st_xsec[350.]=0.474000;
      st_xsec[360.]=0.398000;
      st_xsec[370.]=0.335000;
      st_xsec[380.]=0.285000;
      st_xsec[390.]=0.241000;
      st_xsec[400.]=0.205000;
      st_xsec[410.]=0.176000;
      st_xsec[420.]=0.150000;
      st_xsec[430.]=0.129000;
      st_xsec[440.]=0.111000;
      st_xsec[450.]=0.095200;
      st_xsec[460.]=0.082100;
      st_xsec[470.]=0.071000;
      st_xsec[480.]=0.061500;
      st_xsec[490.]=0.053400;
      st_xsec[500.]=0.046400;
      st_xsec[510.]=0.040400;
      st_xsec[520.]=0.035300;
      st_xsec[530.]=0.030800;
      st_xsec[540.]=0.027000;
      st_xsec[550.]=0.023600;
      st_xsec[560.]=0.020800;
      st_xsec[570.]=0.018200;
      st_xsec[580.]=0.015900;
      st_xsec[590.]=0.014100;
      st_xsec[600.]=0.012400;
      st_xsec[610.]=0.011000;
      st_xsec[620.]=0.009680;
      st_xsec[630.]=0.008570;
      st_xsec[640.]=0.007610;
      st_xsec[650.]=0.006740;
      st_xsec[660.]=0.005980;
      st_xsec[670.]=0.005320;
      st_xsec[680.]=0.004730;
      st_xsec[690.]=0.004250;
      st_xsec[700.]=0.003790;
      st_xsec[710.]=0.003380;
      st_xsec[720.]=0.003020;
      st_xsec[730.]=0.002700;
      st_xsec[740.]=0.002410;
      st_xsec[750.]=0.002160;
      st_xsec[760.]=0.001930;
      st_xsec[770.]=0.001730;
      st_xsec[780.]=0.001550;
      st_xsec[790.]=0.001390;
      st_xsec[800.]=0.001250;
      st_xsec[810.]=0.001120;
      st_xsec[820.]=0.001000;
      st_xsec[830.]=0.000906;
      st_xsec[840.]=0.000811;
      st_xsec[850.]=0.000730;
      st_xsec[860.]=0.000657;
      st_xsec[870.]=0.000593;
      st_xsec[880.]=0.000534;
      st_xsec[890.]=0.000482;
      st_xsec[900.]=0.000435;
      st_xsec[910.]=0.000393;
      st_xsec[920.]=0.000355;
      st_xsec[930.]=0.000321;
      st_xsec[940.]=0.000290;
      st_xsec[950.]=0.000262;
      st_xsec[960.]=0.000238;
      st_xsec[970.]=0.000215;
      st_xsec[980.]=0.000195;
      st_xsec[990.]=0.000176;
      st_xsec[1000.]=0.000160;
      st_xsec[1010.]=0.000145;
      st_xsec[1020.]=0.000131;
      st_xsec[1030.]=0.000119;
      st_xsec[1040.]=0.000108;
      st_xsec[1050.]=0.000098;
      st_xsec[1060.]=0.000089;
      st_xsec[1070.]=0.000081;
      st_xsec[1080.]=0.000073;
      st_xsec[1090.]=0.000067;
      st_xsec[1100.]=0.000060;
      st_xsec[1110.]=0.000055;
      st_xsec[1120.]=0.000050;
      st_xsec[1130.]=0.000045;
      st_xsec[1140.]=0.000041;
      st_xsec[1150.]=0.000038;
      st_xsec[1160.]=0.000034;
      st_xsec[1170.]=0.000031;
      st_xsec[1180.]=0.000028;
      st_xsec[1190.]=0.000026;
      st_xsec[1200.]=0.000023;
      st_xsec[1210.]=0.000021;
      st_xsec[1220.]=0.000019;
      st_xsec[1230.]=0.000018;
      st_xsec[1240.]=0.000016;
      st_xsec[1250.]=0.000015;
      st_xsec[1260.]=0.000013;
      st_xsec[1270.]=0.000012;
      st_xsec[1280.]=0.000011;
      st_xsec[1290.]=0.000010;
      st_xsec[1300.]=0.000009;
      st_xsec[1310.]=0.000008;
      st_xsec[1320.]=0.000008;
      st_xsec[1330.]=0.000007;
      st_xsec[1340.]=0.000006;
      st_xsec[1350.]=0.000006;
      st_xsec[1360.]=0.000005;
      st_xsec[1370.]=0.000005;
      st_xsec[1380.]=0.000004;
      st_xsec[1390.]=0.000004;
      st_xsec[1400.]=0.000004;
      st_xsec[1410.]=0.000003;
      st_xsec[1420.]=0.000003;
      st_xsec[1430.]=0.000003;
      st_xsec[1440.]=0.000003;
      st_xsec[1450.]=0.000002;
      st_xsec[1460.]=0.000002;
      st_xsec[1470.]=0.000002;
      st_xsec[1480.]=0.000002;
      st_xsec[1490.]=0.000002;
      st_xsec[1500.]=0.000001;
      st_xsec[1510.]=0.000001;
      st_xsec[1520.]=0.000001;
      st_xsec[1530.]=0.000001;
      st_xsec[1540.]=0.000001;
      st_xsec[1550.]=0.000001;
      st_xsec[1560.]=0.000001;
      st_xsec[1570.]=0.000001;
      st_xsec[1580.]=0.000001;
      st_xsec[1590.]=0.000001;
      st_xsec[1600.]=0.000001;
      st_xsec[1610.]=0.000001;
      st_xsec[1620.]=0.000000;
      st_xsec[1630.]=0.000000;
      st_xsec[1640.]=0.000000;
      st_xsec[1650.]=0.000000;
      st_xsec[1660.]=0.000000;
      st_xsec[1670.]=0.000000;
      st_xsec[1680.]=0.000000;
      st_xsec[1690.]=0.000000;
      st_xsec[1700.]=0.000000;
      st_xsec[1710.]=0.000000;
      fill=false;
    }
    return st_xsec;
  } 

  float interpolate ( float value, 
      float low, float high, float ylow, float yhigh )
  {
    /* linear interpolation
    float k = (yhigh - ylow) /  (high - low );
    float d = ylow;
    float value = k * ( value - low ) + d;
    */
    float k = log ( yhigh / ylow ) / ( low - high );
    return ylow * exp ( -k * ( value - low ) );
  }

  float getXSec( float m, map < float, float > & xsecs, 
                 XSection::InterpolationType it )
  {
    if ( m < 1. ) return 0.;
    float high=999999.;
    float low=0.;
    for ( map< float, float >::const_iterator i=xsecs.begin();
          i!=xsecs.end() ; ++i )
    {
      if ( fabs ( m - i->first ) < 1. )
      {
        return i->second;
      }

      if ( ( m < i->first ) && ( i->first < high ) )
      {
        high=i->first;
      }

      if ( ( m > i->first ) && ( i->first > low ) )
      {
        low=i->first;
      }
    }
    switch ( it )
    {
      case XSection::Interpolate:
        return interpolate ( m, low, high, xsecs[low], xsecs[high] );
      case XSection::Lower:
        return xsecs[low];
      case XSection::Higher:
        return xsecs[high];
      default:
        return 0.;
    }
  }
}

XSection::XSection()
{
//	  _      ___   __  ______
//	 | |    / _ \  \ \/ / ___|  ___  ___
//	 | |   | | | |  \  /\___ \ / _ \/ __|
//	 | |___| |_| |  /  \ ___) |  __/ (__
//	 |_____|\___/  /_/\_\____/ \___|\___|
//
	xsec_["QCD_MU_PT20TO30"]      = 237000000.; 	//these are eff LO XSEC from https://twiki.cern.ch/twiki/bin/view/Main/EWKMetCommMCDataSets
	xsec_["QCD_MU_PT30TO50"]      = 53200000.;
	xsec_["QCD_MU_PT50TO80"]      = 6340000.;
	xsec_["QCD_MU_PT80TO120"]     = 785000.;
	xsec_["QCD_MU_PT120TO170"]    = 115000.;
	xsec_["QCD_MU_PT170TOInf"]    = 25600.;
	xsec_["INCLUSIVEMU5_PT30"]    = 6.116*pow(10.,7);
	xsec_["INCLUSIVEMU5_PT50"]    = 7.29*pow(10.,6);
	xsec_["INCLUSIVEMU5_PT150"]   = 48100.;
	xsec_["INCLUSIVEMU15"]        = 296.9*pow(10.,6);
	xsec_["WJETS"] 			      = 24170.;
	xsec_["TTBAR"]                = 94.3;
	xsec_["TTBARJETS-MADGRAPH"]   = 95.;
	xsec_["ZJETS-MADGRAPH"]       = 2400.;
	xsec_["PPMUX"]                = 48.44*pow(10.,9);

	//https://twiki.cern.ch/twiki/bin/view/CMS/ProductionReProcessingSpring10#ALPGEN
	//The W*JETS_PT*TO* have a TOtal CS of 25900. The TOtal NLO CS is 31314, hence a common K-facTOr for these samples is 1.20903
	xsec_["W0JETS_PT0TO100"]      = 2.43*pow(10.,+4);
	xsec_["W1JETS_PT0TO100"]      = 8.46*pow(10.,+3);
	xsec_["W2JETS_PT0TO100"]      = 3.50*pow(10.,+3);
	xsec_["W3JETS_PT0TO100"]      = 1.29*pow(10.,+3);
	xsec_["W4JETS_PT0TO100"]      = 4.34*pow(10.,+2);
	xsec_["W5JETS_PT0TO100"]      = 1.31*pow(10.,+2);
	xsec_["W1JETS_PT100TO300"]    = 1.58*pow(10.,+2);
	xsec_["W2JETS_PT100TO300"]    = 2.20*pow(10.,+2);
	xsec_["W3JETS_PT100TO300"]    = 1.66*pow(10.,+2);
	xsec_["W4JETS_PT100TO300"]    = 8.83*pow(10.,+1);
	xsec_["W5JETS_PT100TO300"]    = 3.70*pow(10.,+1);
	xsec_["W1JETS_PT300TO800"]    = 1.72*pow(10.,+0);
	xsec_["W2JETS_PT300TO800"]    = 3.44*pow(10.,+0);
	xsec_["W3JETS_PT300TO800"]    = 3.57*pow(10.,+0);
	xsec_["W4JETS_PT300TO800"]    = 2.47*pow(10.,+0);
	xsec_["W5JETS_PT300TO800"]    = 1.30*pow(10.,+0);
	xsec_["W1JETS_PT800TO1600"]   = 4.01*pow(10.,-3);
	xsec_["W2JETS_PT800TO1600"]   = 9.85*pow(10.,-3);
	xsec_["W3JETS_PT800TO1600"]   = 1.16*pow(10.,-2);
	xsec_["W4JETS_PT800TO1600"]   = 8.62*pow(10.,-3);
	xsec_["W5JETS_PT800TO1600"]   = 4.71*pow(10.,-3);

	xsec_["LM0"]                  = 38.93;
	xsec_["LM1"]                  = 4.888;
	xsec_["LM2"]                  = 0.6027;
	xsec_["LM3"]                  = 3.438;
	xsec_["LM4"]                  = 1.879;
	xsec_["LM5"]                  = 0.473;
	xsec_["LM6"]                  = 0.3104;
	xsec_["LM7"]                  = 1.209;
	xsec_["LM8"]                  = 0.7300;
	xsec_["LM9"]                  = 7.134;
	xsec_["LM10"]                 = 0.04778;
	xsec_["LM11"]                 = 0.8236;
	xsec_["LM12"]                 = 4.414;
	xsec_["LM13"]                 = 6.899;


//	  _____ _ _ _               _____  __  __
//	 |  ___(_) | |_  ___ _ __  | ____|/ _|/ _|
//	 | |_  | | | __|/ _ \ '__| |  _| | |_| |_
//	 |  _| | | | |_|  __/ |    | |___|  _|  _|
//	 |_|   |_|_|\__|\___|_|    |_____|_| |_|
//

	feff_["QCD_MU_PT20TO30"]      = 0.0051;
	feff_["QCD_MU_PT30TO50"]      = 0.0123;
	feff_["QCD_MU_PT50TO80"]      = 0.0219;
	feff_["QCD_MU_PT80TO120"]     = 0.037;
	feff_["QCD_MU_PT120TO170"]    = 0.053;
	feff_["QCD_MU_PT170TOInf"]    = 0.063;
	feff_["INCLUSIVEMU5_PT30"]    = 0.0126;
	feff_["INCLUSIVEMU5_PT50"]    = 0.0246;
	feff_["INCLUSIVEMU5_PT150"]   = 0.056;
	feff_["INCLUSIVEMU15"]        = 0.00037;
	feff_["WJETS"]                = 1.;
	feff_["TTBAR"]                = 1.;
	feff_["TTBARJETS-MADGRAPH"]   = 1.;
	feff_["ZJETS-MADGRAPH"]       = 1.;
	feff_["ppMUX"]                = 0.00176;


	//https://twiki.cern.ch/twiki/bin/view/CMS/ProductionReProcessingSpring10#ALPGEN
	//The W*JETS_PT*TO* have a TOtal CS of 25900. The TOtal NLO CS is 31314, hence a common K-facTOr for these samples is 1.20903
	feff_["W0JETS_PT0TO100"]      = 8.53*pow(10.,-1);
	feff_["W1JETS_PT0TO100"]      = 4.48*pow(10.,-1);
	feff_["W2JETS_PT0TO100"]      = 2.66*pow(10.,-1);
	feff_["W3JETS_PT0TO100"]      = 1.59*pow(10.,-1);
	feff_["W4JETS_PT0TO100"]      = 9.29*pow(10.,-2);
	feff_["W5JETS_PT0TO100"]      = 7.39*pow(10.,-2);
	feff_["W1JETS_PT100TO300"]    = 5.06*pow(10.,-1);
	feff_["W2JETS_PT100TO300"]    = 3.13*pow(10.,-1);
	feff_["W3JETS_PT100TO300"]    = 1.93*pow(10.,-1);
	feff_["W4JETS_PT100TO300"]    = 1.11*pow(10.,-1);
	feff_["W5JETS_PT100TO300"]    = 8.81*pow(10.,-2);
	feff_["W1JETS_PT300TO800"]    = 3.54*pow(10.,-1);
	feff_["W2JETS_PT300TO800"]    = 2.39*pow(10.,-1);
	feff_["W3JETS_PT300TO800"]    = 1.60*pow(10.,-1);
	feff_["W4JETS_PT300TO800"]    = 1.05*pow(10.,-1);
	feff_["W5JETS_PT300TO800"]    = 1.06*pow(10.,-1);
	feff_["W1JETS_PT800TO1600"]   = 2.71*pow(10.,-1);
	feff_["W2JETS_PT800TO1600"]   = 2.02*pow(10.,-1);
	feff_["W3JETS_PT800TO1600"]   = 1.46*pow(10.,-1);
	feff_["W4JETS_PT800TO1600"]   = 9.97*pow(10.,-2);
	feff_["W5JETS_PT800TO1600"]   = 1.22*pow(10.,-1);

	feff_["LM0"]                  = 1.;
	feff_["LM1"]                  = 1.;
	feff_["LM2"]                  = 1.;
	feff_["LM3"]                  = 1.;
	feff_["LM4"]                  = 1.;
	feff_["LM5"]                  = 1.;
	feff_["LM6"]                  = 1.;
	feff_["LM7"]                  = 1.;
	feff_["LM8"]                  = 1.;
	feff_["LM9"]                  = 1.;
	feff_["LM10"]                 = 1.;
	feff_["LM11"]                 = 1.;
	feff_["LM12"]                 = 1.;
	feff_["LM13"]                 = 1.;

//	  _           __            _
//	 | | __      / _| __ _  ___| |_  ___  _ __
//	 | |/ /_____| |_ / _` |/ __| __|/ _ \| '__|
//	 |   <|_____|  _| (_| | (__| |_| (_) | |
//	 |_|\_\     |_|  \__,_|\___|\__|\___/|_|
//
	kfct_["QCD_MU_PT20TO30"]      = 1.; // n.a.
	kfct_["QCD_MU_PT30TO50"]      = 1.; // n.a.
	kfct_["QCD_MU_PT50TO80"]      = 1.; // n.a.
	kfct_["QCD_MU_PT80TO120"]     = 1.; // n.a.
	kfct_["QCD_MU_PT120TO170"]    = 1.; // n.a.
	kfct_["QCD_MU_PT170TOInf"]    = 1.; // n.a.
	kfct_["INCLUSIVEMU5_PT30"]    = 1.; // n.a.
	kfct_["INCLUSIVEMU5_PT50"]    = 1.; // n.a.
	kfct_["INCLUSIVEMU5_PT150"]   = 1.; // n.a.
	kfct_["INCLUSIVEMU15"]        = 79668./(xsec_["INCLUSIVEMU15"]*feff_["INCLUSIVEMU15"]); //from TOpLepTOnPlusJETSRefSel
	kfct_["WJETS"]                = 31314./(xsec_["WJETS"]*feff_["WJETS"]); //from TOpLepTOnPlusJETSRefSel
	kfct_["TTBAR"]                = 157.5/(xsec_["TTBAR"]*feff_["TTBAR"]); //same as below but not listed on TOpLepTOnPlusJETSRefSel
	kfct_["TTBARJETS-MADGRAPH"]   = 157.5/(xsec_["TTBARJETS-MADGRAPH"]*feff_["TTBARJETS-MADGRAPH"]); //from TOpLepTOnPlusJETSRefSel
	kfct_["ZJETS-MADGRAPH"]       = 3048./(xsec_["ZJETS-MADGRAPH"]*feff_["ZJETS-MADGRAPH"]);
	kfct_["PPMUX"]                = 1.; // n.a.

	//https://twiki.cern.ch/twiki/bin/view/CMS/ProductionReProcessingSpring10#ALPGEN
	//The W*JETS_PT*TO* have a TOtal CS of 25900. The TOtal NLO CS is 31314, hence a common K-facTOr for these samples is 1.20903
	kfct_["W0JETS_PT0TO100"]      = 1.20903;
	kfct_["W1JETS_PT0TO100"]      = 1.20903;
	kfct_["W2JETS_PT0TO100"]      = 1.20903;
	kfct_["W3JETS_PT0TO100"]      = 1.20903;
	kfct_["W4JETS_PT0TO100"]      = 1.20903;
	kfct_["W5JETS_PT0TO100"]      = 1.20903;
	kfct_["W1JETS_PT100TO300"]    = 1.20903;
	kfct_["W2JETS_PT100TO300"]    = 1.20903;
	kfct_["W3JETS_PT100TO300"]    = 1.20903;
	kfct_["W4JETS_PT100TO300"]    = 1.20903;
	kfct_["W5JETS_PT100TO300"]    = 1.20903;
	kfct_["W1JETS_PT300TO800"]    = 1.20903;
	kfct_["W2JETS_PT300TO800"]    = 1.20903;
	kfct_["W3JETS_PT300TO800"]    = 1.20903;
	kfct_["W4JETS_PT300TO800"]    = 1.20903;
	kfct_["W5JETS_PT300TO800"]    = 1.20903;
	kfct_["W1JETS_PT800TO1600"]   = 1.20903;
	kfct_["W2JETS_PT800TO1600"]   = 1.20903;
	kfct_["W3JETS_PT800TO1600"]   = 1.20903;
	kfct_["W4JETS_PT800TO1600"]   = 1.20903;
	kfct_["W5JETS_PT800TO1600"]   = 1.20903;

	kfct_["LM0"]                  = 1.41;
	kfct_["LM1"]                  = 1.34;
	kfct_["LM2"]                  = 1.33;
	kfct_["LM3"]                  = 1.40;
	kfct_["LM4"]                  = 1.35;
	kfct_["LM5"]                  = 1.34;
	kfct_["LM6"]                  = 1.30;
	kfct_["LM7"]                  = 1.11;
	kfct_["LM8"]                  = 1.41;
	kfct_["LM9"]                  = 1.48;
	kfct_["LM10"]                 = 1.22;
	kfct_["LM11"]                 = 1.35;
	kfct_["LM12"]                 = 1.34;
	kfct_["LM13"]                 = 1.42;

}

std::ostream& operator<<( std::ostream& out, const XSection& xsec ){
	using namespace std;
	out << "Cross Sections: " << endl;
	map<string, double> xsecs = xsec.getLOXsecs();
	map<string, double> feffs = xsec.getFeffs();
	map<string, double> kfcts = xsec.getKfcts();
	map<string, double>::const_iterator itxsec;
	for (itxsec = xsecs.begin(); itxsec != xsecs.end(); itxsec++)
	{
		string name = itxsec->first;
		out << "  " << name << ": " << flush << endl;
		out << "    LO-X_sec         = " << xsecs[name] << flush << endl;
		out << "    f_eff            = " << feffs[name] << flush << endl;
		out << "    k_factor         = " << kfcts[name] << flush << endl;
		out << "    -> NLO Eff_X_sec = " << xsecs[name] * feffs[name] * kfcts[name] << flush << endl;

	}
	return out;
}

double XSection::getLOXsec( std::string name )
{
  transform ( name.begin(), name.end(), name.begin(), (int(*)(int)) toupper );
  return xsec_[name];
}

std::map<std::string, double> XSection::getLOXsecs() const
{
  return xsec_;
}

double XSection::getFeff( std::string name )
{
  transform ( name.begin(), name.end(), name.begin(), (int(*)(int)) toupper );
  return feff_[name];
}

std::map<std::string, double> XSection::getFeffs() const
{
  return feff_;
}

double XSection::getKfct( std::string name )
{
  transform ( name.begin(), name.end(), name.begin(), (int(*)(int)) toupper );
  return kfct_[name];
}

std::map<std::string, double> XSection::getKfcts() const 
{
  return kfct_;
}

double XSection::getWeight( const std::string & name, double lumi, int nevents ) 
{
  return ( (getNLOEffXsec(name) * lumi) /  (double)nevents);
}
  
double XSection::getNLOEffXsec( std::string name )
{
  transform ( name.begin(), name.end(), name.begin(), (int(*)(int)) toupper );
  return (xsec_[name] * feff_[name] * kfct_[name]);
}
  
void XSection::setEntry ( std::string name, double xsec, double feff, double kfct )
{
  transform ( name.begin(), name.end(), name.begin(), (int(*)(int)) toupper );
  xsec_[name] = xsec;
  feff_[name] = feff;
  kfct_[name] =	kfct;
}

float XSection::mGluino ( float xsec ) const
{
  static map < float, float > gg_xsec=ggs();
  float high = 999999;
  float low  =      0.;
  float xhigh = 0.;
  float xlow = 0.;
  for ( map < float, float >::const_iterator i=gg_xsec.begin(); 
        i!=gg_xsec.end() ; ++i )
  {
    if ( fabs ( xsec - i->second ) < xsec / 1e7 )
    {
      // we are exactly at the right point!
      return i->first;
    }

    if ( ( xsec < i->second ) && ( i->second < high ) )
    {
      high=i->second;
      xhigh = i->first;
    }

    if ( ( xsec > i->second ) && ( i->second > low) )
    {
      low=i->second;
      xlow=i->first;
    }
  }
  if ( low < .00001 || high > 999998. )
  {
    cout << "[XSection] out of range" << endl;
    return 0;
  }
  return interpolate ( xsec, low, high, xlow, xhigh );
}

float XSection::mSquark ( float xsec ) const
{
  static map < float, float > qq_xsec=qqs();
  float high = 999999;
  float low  =      0.;
  float xhigh = 0.;
  float xlow = 0.;
  for ( map < float, float >::const_iterator i=qq_xsec.begin(); 
        i!=qq_xsec.end() ; ++i )
  {
    if ( fabs ( xsec - i->second ) < xsec / 1e7 )
    {
      // we are exactly at the right point!
      return i->first;
    }

    if ( ( xsec < i->second ) && ( i->second < high ) )
    {
      high=i->second;
      xhigh = i->first;
    }

    if ( ( xsec > i->second ) && ( i->second > low) )
    {
      low=i->second;
      xlow=i->first;
    }
  }
  if ( low < .00001 || high > 999998. )
  {
    cout << "[XSection] out of range" << endl;
    return 0;
  }
  return interpolate ( xsec, low, high, xlow, xhigh );
}

float XSection::mStop ( float xsec ) const
{
  static map < float, float > qq_xsec=tts();
  float high = 999999;
  float low  =      0.;
  float xhigh = 0.;
  float xlow = 0.;
  for ( map < float, float >::const_iterator i=qq_xsec.begin(); 
        i!=qq_xsec.end() ; ++i )
  {
    if ( fabs ( xsec - i->second ) < xsec / 1e7 )
    {
      // we are exactly at the right point!
      return i->first;
    }

    if ( ( xsec < i->second ) && ( i->second < high ) )
    {
      high=i->second;
      xhigh = i->first;
    }

    if ( ( xsec > i->second ) && ( i->second > low) )
    {
      low=i->second;
      xlow=i->first;
    }
  }
  if ( low < .00001 || high > 999998. )
  {
    cout << "[XSection] out of range" << endl;
    return 0;
  }
  return interpolate ( xsec, low, high, xlow, xhigh );
}



float XSection::xSecGG( float mgl, InterpolationType it ) const
{
  map < float, float > gg = ggs();
  return getXSec ( mgl, gg, it );
}

float XSection::xSecGQ( float mgl, float msq, InterpolationType it ) const
{
  if ( msq < 1. ) return 0.;
  if ( mgl < 1. ) return 0.;
  
  map< float, map < float, float > > gq_xsec = gqs();
    
  float high = 999999;
  float low  =      0.;
  map < float, float > sqhigh;
  map < float, float > sqlow;

  for ( map< float, map < float, float > >::const_iterator i=gq_xsec.begin();
        i!=gq_xsec.end() ; ++i )
  {
    if ( ( fabs ( i->first - mgl ) < 1. ) )
    {
     // we have an exact gluino mass!
     map < float, float > sq_xsec = i->second;
     cout << "[XSection] ok, we have the right gluino mass " << mgl
          << " now interpolate " << msq << endl;
     return getXSec ( msq, sq_xsec, it );
    }

    if ( ( mgl < i->first ) && ( i->first < high ) )
    {
      high=i->first;
      sqhigh = i->second;
    }

    if ( ( mgl > i->first ) && ( i->first > low ) )
    {
      low=i->first;
      sqlow=i->second;
    }
  }

  /*
  cout << "[XSection] ok gluino mass is between " << low
       << " and " << high << endl;
       */
  float sqlow_v = getXSec ( msq, sqlow, it ); // get the squark-interpolated values
  float sqhigh_v = getXSec ( msq, sqhigh, it ); // get the squark-interpolated values
  // cout << "[XSection] sqlow_v=" << sqlow_v << " sqhigh_v=" << sqhigh_v << endl;
  switch ( it )
  {
    case Interpolate:
      return interpolate ( mgl, low, high, sqlow_v, sqhigh_v );
    case Lower:
      return sqlow_v;
    case Higher:
      return sqhigh_v;
    default:
      return 0.;
  }
}

float XSection::xSecQQ( float msq, InterpolationType it ) const
{
  map < float, float > qq = qqs();
  return getXSec ( msq, qq, it );
}

float XSection::xSecTT( float mst, InterpolationType it ) const
{
  map < float, float > tt = tts();
  return getXSec ( mst, tt, it );
}

float XSection::xSecBB( float mst, InterpolationType it ) const
{
  map < float, float > bb = bbs();
  return getXSec ( mst, bb, it );
}


