#ifdef __cplusplus
extern "C" {
#endif

const char SO_FILE_VERSION[] __attribute__((section(".bugly_version"))) = "<1.1.0>";

#ifdef __cplusplus
}
#endif

#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <dirent.h>
#include "protector.h"
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include "elf.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

typedef void (*handle)(int);

int register_all_func(JNIEnv *env);

void *fetch_so();

bool is_usable_mem_info(const char *mem_info);

jint JNI_OnLoad(JavaVM *vm, void *)
{
    JNIEnv *env = NULL;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }

    LOGD("register func");
    if (register_all_func(env) < 0)
    {
        return -1;
    }

    return JNI_VERSION_1_6;
}

int register_all_func(JNIEnv *env)
{
    JNINativeMethod methods[] = {
        {"load", "()I", (void *)&JNI_FUNC(load)}};

    int len = sizeof(methods) / sizeof(JNINativeMethod);
    DEFINE_REF(jclass, clazz, env, env->FindClass("com/chan/lib/Yasc"));
    return env->RegisterNatives(clazz, methods, len);
}

IMPL_FUNC(jint, load)
{
    char *so = (char *)fetch_so();
    Elf32_Ehdr *header = (Elf32_Ehdr *)so;
    if (!IS_ELF(*header))
    {
        LOGD("can not find so!");
        return -1;
    }

    LOGD("program count %d", header->e_phnum);
    Elf32_Phdr *it = (Elf32_Phdr *)(so + header->e_phoff);
    Elf32_Phdr *end = it + header->e_phnum;

    for (; it != end; ++it)
    {
        LOGD("programe type: %x", it->p_type);
        if (it->p_type == PT_LOAD)
        {
            LOGD("find first load segment");
            break;
        }
    }

    if (it == end) 
    {
        LOGD("can not find load segment");
        return -1;
    }

    return 0;
}

void *fetch_so()
{
    pid_t pid = getpid();
    LOGD("current pid: %d", pid);
    std::ostringstream maps_file_name_os;
    maps_file_name_os << "/proc/" << pid << "/maps";
    std::string maps_file_name = maps_file_name_os.str();

    FILE *maps_file = fopen(maps_file_name.c_str(), "r");
    if (maps_file == NULL)
    {
        LOGI("open %s failed", maps_file_name.c_str());
        return NULL;
    }

    char mem_file_line[1024];
    while (fscanf(maps_file, "%[^\n]\n", mem_file_line) >= 0)
    {
        char mem_address_start_s[10] = {0};
        char mem_address_end_s[10] = {0};
        char mem_info[1024] = {0};
        sscanf(mem_file_line, "%8[^-]-%8[^ ]%*s%*s%*s%*s%s",
               mem_address_start_s,
               mem_address_end_s, mem_info);

        if (!is_usable_mem_info(mem_info))
        {
            continue;
        }

        fclose(maps_file);
        return (void *)strtoul(mem_address_start_s, NULL, 16);
    }

    fclose(maps_file);
    return NULL;
}

bool is_usable_mem_info(const char *mem_info)
{
    if (mem_info == NULL)
    {
        return false;
    }

    size_t mem_info_string_len = strlen(mem_info);
    if (mem_info_string_len == 0)
    {
        return false;
    }

    size_t dot_start_index = strlen(mem_info) - 1;
    for (; dot_start_index != 0; --dot_start_index)
    {
        if (mem_info[dot_start_index] == '/')
        {
            break;
        }
    }

    const char *suffix = mem_info + dot_start_index;
    return !strcmp(suffix, "/libyasc.so");
}
