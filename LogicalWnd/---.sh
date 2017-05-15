echo ===============================
git rm --cached *.user
echo ===============================
git add .
echo ===============================
git commit -m "ignore *.user"
echo ===============================
git push origin master
echo ===============================
read -p "Press any key to continue." var