#include "bro.cpp"
//User of our server
int main()
{
try
{
Bro bro;
bro.setStaticResourcesFolder("static");
typedef struct _student
{
int rollNumber;
char name[21];
char gender;
}Student;
bro.get("/",[](Request& request,Response& response)->void {
response<<R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta charset='utf-8'>
<title>School</title>
</head>
<body>
<h1>XYZ Senior Secondary School</h1><br>
<table border='1'>
<thead>
<tr><th>S.no.</th><th>Roll Number</th><th>Name</th><th>Gender</th><th>Edit</th><th>Delete</th></tr>
</thead>
<tbody>
)"""";
Student stud;
FILE *file=fopen("student.dat","rb");
int sno=0;
char str[20];
if(file!=NULL)
{
while(true)
{
fread(&stud,sizeof(Student),1,file);
if(feof(file)) break;
sno++;
itoa(sno,str,10);
response<<"<tr><td>"<<str<<"</td>";
itoa(stud.rollNumber,str,10);
response<<"<td>"<<str<<"</td>";
response<<"<td>"<<stud.name<<"</td>";
if(stud.gender=='M')
{
response<<"<td><img src='images/male.png' style='height:30px'></td>";
}
else
{
response<<"<td><img src='images/female.png' style='height:30px'></td>";
}
itoa(stud.rollNumber,str,10);
response<<"<td><a href='editStudent?rollNumber="<<str<<"'>Edit</a></td>";
response<<"<td><a href='deleteStudent?rollNumber="<<str<<"'>Delete</a></td></tr>";
}
}
if(sno==0)
{
response<<"<tr><td colspan='6' style='text-align:center'>No Students Found</td></tr>";
}
fclose(file);
response<<R""""(
</tbody>
<div>
<p>why it is above</p>
<a href='StudentAddForm.html'>Add Student</a>
</div>
</body>
</html>
)"""";
response.setContentType("text/html");
});
bro.get("/addStudent",[](Request& request,Response& response)->void {
string rollNumber=request["rollNumber"];
string name=request["name"];
string gender=request["gender"];
Student stud,temp;
int rl=atoi(rollNumber.c_str());
stud.rollNumber=rl;
strcpy(stud.name,name.c_str());
stud.gender=gender[0];
FILE *f=fopen("student.dat","rb");
if(f!=NULL)
{
bool found=false;
while(1)
{
fread(&temp,sizeof(Student),1,f);
if(feof(f)) break;
if(temp.rollNumber==stud.rollNumber)
{
found=true;
break;
}
}
fclose(f);
if(found)
{
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<meta charset='utf-8'>
<head>
<title>School</title>
<script>
function validate(frm)
{
var rollNumber=frm.rollNumber.value.trim();
if(rollNumber.length==0)
{
alert('Roll Number Required');
frm.rollNumber.focus();
return false;
}
var rl=parseInt(rollNumber);
if(isNaN(rl))
{
alert('Roll Number Should be an Integer');
frm.rollNumber.value='';
frm.rollNumber.focus();
return false;
}
var chars='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .';
var name=frm.name.value.trim();
if(name.length==0)
{
alert('Name Required');
frm.name.focus();
return false;
}
for(var i=0;i<name.length;i++)
{
if(chars.indexOf(name.charAt(i))==-1)
{
alert('Incorrect Name Format');
frm.name.value='';
frm.name.focus();
return false;
}
}
return true;
}
</script>
</head>
<body>
<h1>Student (Add Module)</h1>
)"""";
char line[200];
sprintf(line,"<h3 style='color:red'>Roll Number (%d) Has Been Already Alloted</h3>",temp.rollNumber);
response<<line;
response<<"<form action='/addStudent' onsubmit='return validate(this)'>";
sprintf(line,"Roll Number : <input type='text' id='rollNumber' name='rollNumber' value='%d'><br>",temp.rollNumber);
response<<line;
sprintf(line,"Name : <input type='text' id='name' name='name' value='%s'><br>",name.c_str());
response<<line;
response<<"Gender : &nbsp;&nbsp;&nbsp;";
if(gender[0]=='M')
{
response<<"<input type='radio' name='gender' id='male' value='M' checked>Male &nbsp&nbsp;<input type='radio' name='gender' id='female' value='F'>Female<br>";
}
else
{
response<<"<input type='radio' name='gender' id='male' value='M'>Male &nbsp;&nbsp;<input type='radio' name='gender' id='female' value='F' checked> Female <br>";
}
response<<R""""(
<button type='submit'>Save</button>
</form>
</body>
</html>
)"""";
response.setContentType("text/html");
return;
}
}
f=fopen("student.dat","ab");
if(f!=NULL)
{
fwrite(&stud,sizeof(Student),1,f);
fclose(f);
}
response<<R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>School</title>
</head>
<body>
<h1>Student (Add Module)</h1>
<br>
<h3>Student Added Successfully</h3>
<form action='/'>
<button type='submit'>OK</button>
</form>
<a href='/'>Home</a>
</body>
</html>
)"""";
response.setContentType("text/html");
});
bro.listen(6060,[](Error& error){
if(error.hasError())
{
cout<<error.getError()<<endl;
return;
}
cout<<"Bro HTTP server is listening on port 6060...."<<endl;
});
}catch(string exception)
{
cout<<exception<<endl;
}
return 0;
}