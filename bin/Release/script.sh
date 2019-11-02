echo '1 - Shamir`s
2 - Diffi-Hellman`s
3 - RSA
4 - El-Ghamal`s
Enter type of simulated encryption algorythm: '
read T

if [ $T ==  2 ]
then
#( read OUTPUT1 <<< $(echo "$T 1 " | ./001.exe | grep ' secret number is ')  ) &
#read OUTPUT2 <<< $(echo "$T 2 " | ./001.exe | grep ' secret number is ')
#echo "$OUTPUT1 output1"
#echo "$OUTPUT2 output2"
echo "$T 1 " | ./001.exe | grep ' secret number is ' &
echo "$T 2 " | ./001.exe | grep ' secret number is '

else

echo "$T 1 " | ./001.exe > /dev/null &
echo "$T 2 " | ./001.exe > /dev/null

if [ -e ./.dispatcher ]
then
rm ./.dispatcher
fi
if [ -e ./.recipient ]
then
rm ./.recipient
fi

fi