#include "h_files.h"
int log_function(int num)
{
	int ans=0;
	if(num==0)
		return 1;
	else
	{
		while(num>0)
		{
			ans++;
			num=num/10;
		}
		return ans;
	}
}
