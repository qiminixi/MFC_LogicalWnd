set -xv
git rm --cached *.user
git add .
git commit -m "ignore *.user"
git push origin master

read -p "Press any key to continue." var