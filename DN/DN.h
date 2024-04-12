#pragma once
#ifdef __cplusplus
extern "C" {
#endif

	void* DNEngineHandleCreate();

	int	  DNEngineProcess(void* hEngineHandle, unsigned char* pbGray, int w, int h);

	void  DNEngineHandleDestroy(void* hEngineHandle);

#ifdef __cplusplus
}
#endif