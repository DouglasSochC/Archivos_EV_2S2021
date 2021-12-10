#ifndef ADM_CAP_H
#define ADM_CAP_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class adm_cap
{
public:

    void mkdir(map<string, string> param_got, disco::User UserLoggedIn);
    void cat(map<string, string> param_got, disco::Mount partitionMount, disco::User UserLoggedIn);

    disco::Superblock getSPB(int );
    void readInodeArchive(int position);
};

#endif //ADM_CAP_H