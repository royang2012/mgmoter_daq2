// In the low-level function LLGetBitStatus, a signed int is returned. This function convert the signed 
// int into binary and return the 28th bit of the binary array to indicate the motion of motor 
//
//	Return value: 1 = moving, 0 = not moving
//

#include "stdafx.h"
#include <math.h>

bool Boolean28th(long status){
	unsigned long value;
	int i;
	bool flag[32];
	if (status < 0) {
		flag[0] = 1;
		status = status + pow(2.0, 31);
	}
	else flag[0] = 0;
	for (i = 1; i < 32; i++){
		value = pow(2.0, (31 - i));
		if (status >= value)
			flag[i] = 1;
		else flag[i] = 0;
		status = status - flag[i] * value;
	}
	return !flag[28];
}