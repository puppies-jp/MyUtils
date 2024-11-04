#include <errno.h>
#include <stdio.h>
#include <string.h>

#define FUSE_USE_VERSION 31
#include <fuse.h>

#define MYFS_FILENAME "hello"
#define MYFS_MESSAGE "Helo,Binary Hacks!\n"

/*
# ビルド
gcc ../src/fuse-sample.c `pkg-config fuse3 --cflags --libs` -o myfs

# 指定したフォルダにマウント
./myfs ./test/

# マウントしたフォルダにアクセス
ls ./test

# アンマウント
umount ./test
*/

// ファイルシステム初期化
static void *my_init(struct fuse_conn_info *conn,struct fuse_config *cfg){
    return NULL;
}

// getattr
static int my_getattr(const char *path,struct stat *stbuf,struct fuse_file_info *fi){
    int res = 0;
    memset(stbuf,0 ,sizeof(struct stat));
    if(strcmp(path,"/") == 0){
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }else if(strcmp(path + 1,MYFS_FILENAME) == 0){
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = sizeof(MYFS_MESSAGE);
    }else{
        res = -ENOENT;
    }
    return res;
}

// readdir
static int my_readdir(const char *path,void *buf,fuse_fill_dir_t filler,
off_t offset,struct fuse_file_info *fi,enum fuse_readdir_flags flags){
    if(strcmp(path,"/") != 0){
        return -ENOENT;
    }

    filler(buf,".",NULL,0,0);
    filler(buf,"..",NULL,0,0);
    filler(buf,MYFS_FILENAME,NULL,0,0);
    return 0;

}

// open
static int my_open(const char *path,struct fuse_file_info *fi){
    if(strcmp(path +1, MYFS_FILENAME) != 0){
        return -ENOENT;
    }

    if((fi->flags & O_ACCMODE) != O_RDONLY){
        return -EACCES;
    }

    return 0;
}

// read
static int my_read(const char *path,char *buf,size_t size,off_t offset,struct fuse_file_info *fi){
    if(strcmp(path +1, MYFS_FILENAME) != 0){
        return -ENOENT;
    }

    size_t len = sizeof(MYFS_MESSAGE);
    if(len <= offset){
        return 0;
    }

    if(len < offset +size){
        size = len -offset;
    }

    memcpy(buf,MYFS_MESSAGE + offset , size);
    return size;
}

static const struct fuse_operations my_fuse_ops = {
    .init = my_init,
    .getattr = my_getattr,
    .readdir = my_readdir,
    .open = my_open,
    .read = my_read
};

int main(int argc,char* argv[]){
    int ret;
    struct fuse_args args = FUSE_ARGS_INIT(argc,argv);

    ret = fuse_main(args.argc, args.argv, &my_fuse_ops, NULL);
    fuse_opt_free_args(&args);

    return ret;
}