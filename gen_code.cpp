#include <cstring>
#include <iostream>
int main(){
int status = 2;
char buf[256];
std::cin>>buf;
int cursor = 0;
	while(buf[cursor] != '\0'){
switch(status){
	case 1:{
	switch(buf[cursor]){
		case 'a':{
printf("No such transition from %d by %c",1, buf[cursor]);
exit(1);
break;
}
		case 'b':{
printf("No such transition from %d by %c",1, buf[cursor]);
exit(1);
break;
}
		case 'c':{
printf("No such transition from %d by %c",1, buf[cursor]);
exit(1);
break;
}
		case 'd':{
cursor++;
status = 1;
break;
}
default:
printf("No such symbol %c", buf[cursor]);
exit(1);break;
}
break;
}
	case 2:{
	switch(buf[cursor]){
		case 'a':{
cursor++;
status = 3;
break;
}
		case 'b':{
printf("No such transition from %d by %c",2, buf[cursor]);
exit(1);
break;
}
		case 'c':{
printf("No such transition from %d by %c",2, buf[cursor]);
exit(1);
break;
}
		case 'd':{
printf("No such transition from %d by %c",2, buf[cursor]);
exit(1);
break;
}
default:
printf("No such symbol %c", buf[cursor]);
exit(1);break;
}
break;
}
	case 3:{
	switch(buf[cursor]){
		case 'a':{
printf("No such transition from %d by %c",3, buf[cursor]);
exit(1);
break;
}
		case 'b':{
cursor++;
status = 4;
break;
}
		case 'c':{
cursor++;
status = 5;
break;
}
		case 'd':{
printf("No such transition from %d by %c",3, buf[cursor]);
exit(1);
break;
}
default:
printf("No such symbol %c", buf[cursor]);
exit(1);break;
}
break;
}
	case 4:{
	switch(buf[cursor]){
		case 'a':{
printf("No such transition from %d by %c",4, buf[cursor]);
exit(1);
break;
}
		case 'b':{
printf("No such transition from %d by %c",4, buf[cursor]);
exit(1);
break;
}
		case 'c':{
printf("No such transition from %d by %c",4, buf[cursor]);
exit(1);
break;
}
		case 'd':{
cursor++;
status = 1;
break;
}
default:
printf("No such symbol %c", buf[cursor]);
exit(1);break;
}
break;
}
	case 5:{
	switch(buf[cursor]){
		case 'a':{
printf("No such transition from %d by %c",5, buf[cursor]);
exit(1);
break;
}
		case 'b':{
printf("No such transition from %d by %c",5, buf[cursor]);
exit(1);
break;
}
		case 'c':{
printf("No such transition from %d by %c",5, buf[cursor]);
exit(1);
break;
}
		case 'd':{
cursor++;
status = 1;
break;
}
default:
printf("No such symbol %c", buf[cursor]);
exit(1);break;
}
break;
}
break;
}
}
if(status != 1)	printf("Failed to accept, NOT match");
else
	printf("Program match %s", buf);
return 0;
}