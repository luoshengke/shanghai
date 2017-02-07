//version 2012-05-21, using win32 sdk replace c-runtime function.

#ifndef NEW_MOLDDM_SOFT_LIC_V8
#define NEW_MOLDDM_SOFT_LIC_V8


#ifdef __cplusplus
extern "C" {
#endif

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Cryptst.lib")


extern void get_soft_lic_error_msg( char *lic_err_msg );
// 0: ��ȷ���޴���
// -1������ID�Ų���ȷ
// -2: ���ڲ��Ϸ�
// -3: ���ڹ���
// -4: �����ļ��򲻿�
// -5: �޶��壬����
// -6: �����ļ���1��д�򲻿�
// -7: �����ļ���X��д�򲻿�
// -8: ���ڹ��ڣ��ѱ��
// -9: �޴�license

extern int check_molddm_license( char *lic_fn_path );
//return 0: error!, return 1: ok!

extern int get_expiration_date( );
//����ֵ��������������������, ���Ϊ���������ʾ�ѹ��ڡ�ע�⣺����������ʾ�����˶�����

#ifdef __cplusplus
}
#endif



#endif /* NEW_MOLDDM_SOFT_LIC_V8 */
