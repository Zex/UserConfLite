#include <fstab.h>
#include "errhdr.h"
//       void endfsent(void);
//       struct fstab *getfsent(void);
//       struct fstab *getfsfile(const char *mount_point);
//       struct fstab *getfsspec(const char *special_file);
//       int setfsent(void);
//
//           struct fstab {
//               char       *fs_spec;       /* block device name */
//               char       *fs_file;       /* mount point */
//               char       *fs_vfstype;    /* file-sysem type */
//               char       *fs_mntops;     /* mount options */
//               const char *fs_type;       /* rw/rq/ro/sw/xx option */
//               int         fs_freq;       /* dump frequency, in days */
//               int         fs_passno;     /* pass number on parallel dump */
//           };

int main()
{
    struct fstab* ent;

    if (1 != setfsent())
    {
        LOG_ERR("")
        return 0;
    }

    while ((ent = getfsent()))
    {
        std::cout 
            << "\nfs_spec: " << ent->fs_spec
            << "\nfs_file: " << ent->fs_file
            << "\nfs_vfstype: " << ent->fs_vfstype
            << "\nfs_mntops: " << ent->fs_mntops
            << "\nfs_type: " << ent->fs_type
            << "\nfs_freq: " << ent->fs_freq
            << "\nfs_passno: " << ent->fs_passno
            << "\n******************************************\n";
    }

    endfsent();

    return 0;
}

