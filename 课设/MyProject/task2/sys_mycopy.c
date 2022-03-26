asmlinkage long sys_mycopy(const char *src_file,const char *copy_file){
        int infd,outfd,count;
        char buf[256];
        mm_segment_t fs;
        fs = get_fs();//获取当前线程的thread_info->addr_limit
        set_fs(get_ds());//将能访问的空间thread_info->addr_limit扩大到KERNEL_DS
        if((infd=sys_open(src_file,O_RDONLY,0)) == -1)    {
            return 1;
        }
        if((outfd=sys_open(copy_file,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR))== -1){
            return 2;
        }
        while((count = sys_read(infd,buf,256))>0)
        {
            if(sys_write(outfd,buf,count)!= count)
                return 3;
        }
        if(count == -1) return 4;
        sys_close(infd);
        sys_close(outfd);
        set_fs(fs);//重新设置访问地址限制
        return 0;
 }