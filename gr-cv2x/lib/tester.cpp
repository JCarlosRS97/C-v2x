#include <iostream>
#include <cmath>
#include <complex>
// int main(){
//   std::vector<int> v;
//   SL_V2XCommunication sl;
//   printf("Objeto creado\n");
//   sl.getV2XCommResourcePool();
//   printf("Resource pool obtenido\n");
//   LTEv::SL_V2XUEConfig sL_V2XUEConfig(200, 1, 0, 1);
//   sl.PSxCH_Procedures(sL_V2XUEConfig, 0);
//   printf("Formato de transmision\n");
// }

int main(){
   int slssId = 301;
   int ZC_roots[2] = {26, 37};
   std::complex<float> psss_symbols[62], ssss_symbols[62];
   int u = (slssId > 167)? ZC_roots[1] : ZC_roots[0];

   for(int i = 0; i <= 30; i++){
      psss_symbols[i] = std::polar(1.0f, float(-M_PI*u*i*(i+1.0)/63.0));
      psss_symbols[i] *= sqrt(72/62.0);
      // std::cout<<psss_symbols[i]<<std::endl;
   }
   for(int i = 31; i <= 61; i++){
      psss_symbols[i] = std::polar(1.0f, float(-M_PI*u*(i + 1.0)*(i + 2.0)/63.0));
      psss_symbols[i] *= sqrt(72/62.0);
      // std::cout<<psss_symbols[i]<<std::endl;
   }
   //TS 36.211 6.11.2.1 with suftrame 5
   int x[31];
   int s[31];
   int c[31];
   int z[31];

   int n_id_1 = slssId % 168;
   int n_id_2 = slssId / 168;
   int q_prime = n_id_1 / 30;
   int q = (n_id_1 + float(q_prime*(q_prime + 1))/2)/30;
   int m_prime = n_id_1 + q*(q + 1)/2.0f;
   int m0 = m_prime % 31;
   int m1 = (m0 + m_prime/31 + 1) % 31;

   //s_m1 sequence
   //x and s sequences of s_m1
   x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[4] = 1;
   s[0] = 1; s[1] = 1; s[2] = 1; s[3] = 1; s[4] = -1;
   for (int  i = 0; i <= 25; i++){
      x[i + 5] = (x[i + 2] + x[i]) % 2;
      s[i + 5] = 1 - 2*x[i + 5];
   }

   int s_m0[31];
   int s_m1[31];
   for(int i = 0; i < 31; i++){
      s_m0[i] = s[(i + m0) % 31];
      s_m1[i] = s[(i + m1) % 31];
   }

   // c0 y c1 sequences
   //x and c sequences of c0 and c1
   x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[4] = 1;
   c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1; c[4] = -1;
   for (int  i = 0; i <= 25; i++){
      x[i + 5] = (x[i + 3] + x[i]) % 2;
      c[i + 5] = 1 - 2*x[i + 5];
   }

   int c0[31];
   int c1[31];
   for(int i = 0; i < 31; i++){
      c0[i] = c[(i + n_id_2) % 31];
      c1[i] = c[(i + n_id_2 + 3) % 31];
   }

   //z1_m1 sequence
   //x and c sequences of s_m1
   x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[4] = 1;
   z[0] = 1; z[1] = 1; z[2] = 1; z[3] = 1; z[4] = -1;
   for (int  i = 0; i <= 25; i++){
      x[i + 5] = (x[i + 4] + x[i + 2] + x[i + 1] + x[i]) % 2;
      z[i + 5] = 1 - 2*x[i + 5];
   }

   int z1_m1[31];
   for(int i = 0; i < 31; i ++){
      z1_m1[i] = z[(i + (m1 % 8)) % 31];
   }
   //final sequence
   for(int i = 0; i < 31; i++){
      ssss_symbols[2 * i] = s_m1[i] * c0[i] * sqrt(72/62.0);
      ssss_symbols[2 * i  + 1] = s_m0[i] * c1[i] * z1_m1[i] * sqrt(72/62.0);
   }
   //
   // for(int i = 0; i < 62; i++){
   //    std::cout<<ssss_symbols[i]<<std::endl;
   // }
   const int NSLRB = 6;
   const int NSLsymb = 7;
   const int NRBsc = 12;
   int NSLsc = NSLRB*NRBsc;
   std::complex<float> subframe[NSLsc*14];
   int frecPos = NSLsc/2 - 31;
   std::cout << frecPos << '\n';

   for(int i = 0; i < 62; i++){
      subframe[1*NSLsc + frecPos] = psss_symbols[i];
      subframe[2*NSLsc + frecPos] = psss_symbols[i];
      subframe[11*NSLsc + frecPos] = ssss_symbols[i];
      subframe[12*NSLsc + frecPos] = ssss_symbols[i];
      frecPos += 1;
   }
   for(int i = 0; i < NSLsc; i++){
      for(int k = 0; k < 14; k++){
         std::cout << subframe[k * NSLsc + i] << ' ';
      }
         std::cout<< '\n';

   }
}
