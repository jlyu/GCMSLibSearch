#ifdef MATCHAPI

#else
#define  MATCHAPI extern "C" __declspec(dllimport)
#endif

/************************************************************************/
/* 功能：谱图对比
   参数：
   Mass_Match：待匹配质量数
   AjustedAbundance_Match：待匹配质量数丰度
   MassCount_Match：待匹配峰个数
   Mass_Lib：库谱图质量数
   AjustedAbundance_Lib：库谱图质量数丰度
   MassCount_Lib：库谱图峰个数
   返回值：
   -1：匹配失败
   说明：输入的Mass_Match和Mass_Lib需要是质量数从小到大排列。
*/
/************************************************************************/
MATCHAPI unsigned int
	DiffSpectrum(unsigned int* Mass_Match, float* AjustedAbundance_Match, unsigned int MassCount_Match,
	unsigned int* Mass_Lib, float* AjustedAbundance_Lib, unsigned int MassCount_Lib);