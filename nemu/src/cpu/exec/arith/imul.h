#ifndef __IMUL_H__
#define __IMUL_H__

make_helper(imul_rm2a_b);//f6 5

make_helper(imul_rm2a_v);  //f7 5
make_helper(imul_rm2r_v); //0f af
make_helper(imul_si_rm2r_v);//6b
make_helper(imul_i_rm2r_v); //69

#endif
