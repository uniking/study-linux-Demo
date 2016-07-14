s_pwd=`pwd`
sed "1c\\%define _topdir ${s_pwd}" ./SPECS/fdaemond.demo >./SPECS/fdaemond.spec
