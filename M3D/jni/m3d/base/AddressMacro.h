
#ifndef  M3D_ADDRESS_MACROS_H
#define  M3D_ADDRESS_MACROS_H

#define M3D_ADDRESSTYPE unsigned long long 
template<class T> M3D_ADDRESSTYPE TAddress(T& a) {
	//��ȡ��ַ����ʵ��ʽ
	T* boxAddress = &a;
	M3D_ADDRESSTYPE address = ((M3D_ADDRESSTYPE*)(&boxAddress))[0];
	return address;
}
template<class T> T* TFromAddress(M3D_ADDRESSTYPE address) {
	//��ȡ��ַ����ʵ��ʽ
	T* boxAddress = (T*)address;
	return boxAddress;
}

#endif 