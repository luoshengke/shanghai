//version 2012-05-21, using win32 sdk replace c-runtime function.

#ifndef NEW_MOLDDM_SOFT_LIC_V8
#define NEW_MOLDDM_SOFT_LIC_V8


#ifdef __cplusplus
extern "C" {
#endif

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Cryptst.lib")


extern void get_soft_lic_error_msg( char *lic_err_msg );
// 0: 正确，无错误
// -1：机器ID号不正确
// -2: 日期不合法
// -3: 日期过期
// -4: 密码文件打不开
// -5: 无定义，保留
// -6: 暗标文件第1次写打不开
// -7: 暗标文件第X次写打不开
// -8: 日期过期，已标记
// -9: 无此license

extern int check_molddm_license( char *lic_fn_path );
//return 0: error!, return 1: ok!

extern int get_expiration_date( );
//返回值：正数：还需多少天过期, 如果为负数，则表示已过期。注意：负数并不表示过期了多少天

#ifdef __cplusplus
}
#endif



#endif /* NEW_MOLDDM_SOFT_LIC_V8 */
