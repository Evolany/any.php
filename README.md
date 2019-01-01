
A extremely simple &amp; fast php web framework

# Goals
### Extremely Fast
* Almost As fast as non-framework to handle JSON request.
* < 14 ms under 100*10 requests.
* Test with db query &amp; 7.5kb json response. using ApacheBench

### Extremely Easy to Use
* Only 1 file, with about 25 functions to remember.

# Features.

### RESTful Dispatching 
* /{$controller}/{$action}/{$ID}
* /@{$scheme_name}?{$query_conditions}

### HTML Template & Custom Tags
* 3~5 times faster than Smarty
* Only about 10 tags to remember.
* Define your own tags easily

### Simple &amp; Fast DB access
* No need to define any model, just provide us a scheme file. we do the rest things for you. 

### Auto RESTful response.
* No controller, no model, all you need to do is just to define a db scheme and permissions.

### L2 Cache
* ARC + memcached

### Unix-like Permission check.
* Check all controller/actions permission with annotation
* Declare permissions like unix command line.

### Annotations &amp; PHPDoc
* Support variable custom annotations.
* Generate PHP Doc automatically

### Filters like Servlet
* Support filters like Java-Servlet to handle auth / oauth process.

### Auto Unit Test
* Using csv files to define test cases only.
* Auto test both JSON and HTML.

### Tools
* Mail
* CSV
* GD2 (Image processing)
* S3
* SEO
* Binary 

---

# Requirement

* PHP 5.4 (with APC, GD2, Memcache(Memcached), PDO, pdo-mysql ...)
  or PHP 7.2+ with APCu ...
* Memcached

# Installation
Check out from here
```sh
cd YOUR_HTTP_DOCUMENT_ROOT 
git clone https://github.com/Evolany/any.php anyphp
```

# Hello world
Create project
```sh
cd YOUR_HTTP_DOCUMENT_ROOT/anyphp 
./anyphp.php create myproj 
cd ../myproj
```

### Configurations
```sh
vi conf/conf.inc
```
Change the following lines with your own
```php
static $db_engine = "mysql";
static $db_host	= "127.0.0.1";	
static $db_port	= "3306"; 
static $db_name	= "example";
static $db_user	= "root";
static $db_pass	= "root";
static $cache_hosts	= "localhost";	
static $cache_port	= "11211";
```

### DB migration (Optional)
```sh
php any.php migrate #run this under your project folder
```

### Configure your Apache/Nginx
Here is an example for Apache.<BR>
Add these rows to your httpd.conf (or extra/httpd-vhosts.conf)
```ini
<VirtualHost *:80> 
DocumentRoot "YOUR_PROJECT_PATH" 
Options FollowSymLinks 
ServerName YOUR_PROJECT_NAME.test 
RewriteEngine on 
RewriteRule images/(.*)$ /webroot/images/$1 [L] 
RewriteRule css/(.*)$ /webroot/css/$1 [L] 
RewriteRule js/(.*)$ /webroot/js/$1 [L] 
RewriteRule ^(.*)\.html$ /webroot/html/$1.htm [L] 
RewriteRule !\.(php|svg|ttf|htc|ico|gif|png|jpg|jpeg|css|js|swf|html|htm|json)$ /any.php?__URL__=%{REQUEST_URI} [QSA,NE,L]
</VirtualHost>
```

### Add Virtual Hostname
Add virtural host IP to your hosts file
```bash
# add this line to /etc/hosts 
127.0.0.1	YOUR_PROJECT_NAME.test
```

### Restart your apache
```bash
sudo apachectl restart
```

### Check the result 
Try it by accessing [http://YOUR_PROJECT_NAME.test] 


# RESTful dispatching
| URI Pattern	| HTTP Method | Delegate File | Delegate Function |
|-----|-----|-----|-----|
| [/PATH]/CONTROLLER_NAME/[/ID] | GET<br>POST<br>PUT<br>DELETE | controllers/CONTROLLER_NAME.inc | get(\$params)<br>@see example_1<br>post(\$params)<br>@see example_2<br>put(\$params)<br>delete(\$params) |
| [/PATH]/CONTROLLER_NAME/[/ID]/ACTION_NAME/[/ID] | GET<br>POST<br>PUT<br>DELETE | controllers/CONTROLLER_NAME.inc | ACTION_NAME($params)<BR>@see example_3 |
| /@SCHEMA_NAMEs/ | GET | delegate/RestfulDelegate.inc | get list of this resource |
| /@SCHEMA_NAME/[/ID] | GET<br>POST<br>PUT<br>DELETE | delegate/RestfulDelegate.inc | CRUD of this resource |

### Example 1 : 
Call controller with default action using GET method

the relative URI
```bash
curl ${YOUR_HOSTNAME}/mycontroller?id=1&name=sss
```
Your controller file 
```bash
${PROJECT_HOME}/controllers/mycontroller.inc
```

```php
<?php 
function get($params){ //all request parameters are encapsulated in $params 
  //also you can use render_html or render_text to send plain text 
  render_json($params); 
} 
```
The result will be :
```json
{"id":1,"name":"sss"}
```

### Example 2 : 
Call controller with default action using POST method

in your js (Example of JQuery)
```javascript
$.post('/mycontroller',{id:1,title:"blah blah..."}); 
```
in your controller
```bash
${PROJECT_HOME}/controllers/mycontroller.inc 
```
```php
<?php 
function post($params){ 
    //all request parameters are encapsulated in $params 
    render_json($params); //params = {id:1,title:"blah blah..."} 
}
```
The result
```json
{id:1,title:"blah blah..."} 
```

### Example 3 : 
Call controller with action name

```bash
curl ${YOUR_HOSTNAME}/mycontroller/myaction?name=php 
```
in your controller
```bash
controllers/mycontroller.inc
```
```php
<?php 
function myaction($params){ 
    render_text($params["name"]); 
} 
#the result will be "php"
```


## DB & Restful Requests

| Command(URL) | Alias(PHP) | Syntax | Example | Description |
|---|---|---|---|---|
| eq | = | \$COLUMN_NAME=${v} | | Equal |
| ne | ! | \$COLUMN_NAME@ne=${v} | | Not Equal |
| lt | < | \$COLUMN_NAME@lt=${v} | | less than |
| le | <= | \$COLUMN_NAME@le=${v} | | less or equal |
| gt | > | \$COLUMN_NAME@gt=${v} | | greater than |
| ge | >= | \$COLUMN_NAME@ge=${v} | | greater or equal |
| in | [] | \$COLUMN_NAME@in=${v} | | in |
| nin | ![] | \$COLUMN_NAME@nin=${v} | | not in |
| bt | () | \$COLUMN_NAME@ne=${v} | | Not Equal |
| nb | !() | \$COLUMN_NAME@ne=${v} | | Not Equal |
| l | ? | \$COLUMN_NAME@l=${v} | | like |
| nl | !? | \$COLUMN_NAME@nl=${v} | | Not like |
| m | ~ | \$COLUMN_NAME@m=${v} | | RegExp match |
| nm | !~ | \$COLUMN_NAME@nm=${v} | | RegExp not match |
| mi | ~~ | \$COLUMN_NAME@mi=${v} | | RegExp match ignore case |
| nmi | !~~ | \$COLUMN_NAME@nmi=${v} | | RegExp not match ignore case |


### eq (equal)
```bash
# URL :
curl http://HOSTNAME/@tasks?id=5
```
```php
//php
$res = db_find("tasks", ['id'=>5]);
```
```sql
/*Equavalent SQL*/
select * from tasks where id=5;
```

### ne	! (Not Equal)
```bash
# URL :
/@tasks?id@ne=5
```
```php
//PHP
db_find("tasks", ["id@!"=>5]);	//not equal 
```
```SQL
-- SQL
select * from tasks where id != 5;
```

### lt < (less than)	
```bash
# URL :
/@tasks?id@lt=5
```
```php
//PHP
db_find("tasks", ["id@<"=>5]);	//not equal 
```
```SQL
-- SQL
select * from tasks where id < 5;
```

### gt	> (greater than);	
```bash
# URL :
# syntax : ${column_name}@gt=${v} 
/@tasks?id@gt=5
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@>" => ${v} ]	)
db_find("tasks", ["id@>"=>5]);	//not equal 
```
```SQL
-- SQL
select * from tasks where id > 5;
```
### le	<= (less or equal)	
```bash
# URL :
# syntax : ${column_name}@le=${v} 
/@tasks?id@le=5
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@<=" => ${v} ]	)
db_find("tasks", ["id@≤"=>5]);
```
```SQL
-- SQL
select * from tasks where id <= 5;
```

### ge	>= (greater or equal)
```bash
# URL :
# syntax : ${column_name}@ge=${v} 
/@tasks?id@ge=5
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@>=" => ${v} ])
db_find("tasks", ["id@>="=>5]);
```
```SQL
-- SQL
select * from tasks where id >= 5;
```

### in	[]	(sql IN clause)
```bash
# URL :
# syntax : ${column_name}@in=${v1,v2 ...} 
/@tasks?id@in=5,6
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@[]" => ${v1,v2 ...} ])
db_find("tasks", ["id@[]"=>[5,6]]);
```
```SQL
-- SQL
select * from tasks where id in (5,6);
```

### nin	![]	(sql not in)
```bash
# URL :
# syntax : ${column_name}@nin=${v1,v2 ...} 
/@tasks?id@nin=5,6
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@![]" => ${v1,v2 ...} ])
db_find("tasks", ["id@![]"=>[5,6]]);
```
```SQL
-- SQL
select * from tasks where id not in (5,6);
```

### bt  ()	(sql between)
```bash
# URL :
# syntax : ${column_name}@bt=${v1,v2 ...} 
/@tasks?id@bt=5,20
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@bt" => ${v1,v2 ...} ])
db_find("tasks", ["id@()"=>[5,20]]);
```
```SQL
-- SQL
select * from tasks where id between 5 and 20;
```

### nb	!()	(sql not between)
```bash
# URL :
# syntax : ${column_name}@nb=${v1,v2 ...} 
/@tasks?id@nb=5,20
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@nb" => ${v1,v2 ...} ])
db_find("tasks", ["id@!()"=>[5,20]]);
```
```SQL
-- SQL
select * from tasks where id not between 5 and 20;
```

### l	?	(sql like)
```bash
# URL :
# syntax : ${column_name}@l=${v}  
/@tasks?title@l=abc
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@?" => ${v} ])
db_find("tasks", ["title@?"=>"abc"]);
```
```SQL
-- SQL
select * from tasks where title like '%abc%';
```

### nl	!?	
```bash
# URL :
# syntax : ${column_name}@nl=${v}  
/@tasks?title@nl=abc
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@!?" => ${v} ])
db_find("tasks", ["title@!?"=>"abc"]);
```
```SQL
-- SQL
select * from tasks where title not like '%abc%';
```

### m   ~	
```bash
# URL :
# syntax : ${column_name}@m=${v}  
/@tasks?title@m=abc
```
```php
//PHP
/*
Syntax :  db_find($table_name, [ "${column_name}@~" => ${v} ])
mysql/postgres only 
For MySQL : these are available : 
^ , $ , . , [...] , [^...] , p1|p2|p3 , * , + , {n} , {m,n} 
*/

db_find("tasks", ["title@~"=>"abc|bcd"]);
```
```SQL
-- SQL
select * from tasks where title REGEXP 'abc|bcd';
```


### nm	!~	
```bash
# URL :
# syntax : ${column_name}@nm=${v}  
/@tasks?title@nm=abc
```
```php
//PHP
//Syntax :  db_find($table_name, [ "${column_name}@!~" => ${v} ])
db_find("tasks", ["title@!~"=>"abc|bcd"]);
```
```SQL
-- SQL
select * from tasks where title NOT REGEXP 'abc|bcd';
```

## Use schema file

@see : [general][restful] of chapter DB, Model, Schema


# Dispatching


### Example 4 : 
Use automatical RESTful mechanism to GET data <br>
1. Get newest 20 tasks from tasks table 
```bash
curl http://myproj.test/@tasks?order=desc&sort=updAt&limit=20 
```
2. Get tasks that title like '%test%' and register after 2013-10-30 
```bash
curl http://myproj.test/@tasks?title@l=test&regAt@gt=2013-10-30 
```
3. Get task with id=35
```bash
curl http://myproj.test/@tasks/35 
```
you don't need to write server code, `any.php` does this for you.


### Example 5 : 
Use automatical RESTful mechanism to POST new record

in your JS (example with JQuery) 
```Javascript
$.post('/@tasks',{title:"blah blah..."}); 
```
the result will be 
```json
{id:75, title:"blah blah... "}
```
75 is the last insert id <br>
you don't need to write server code, `any.php` does this for you.


### Example 6 : 
Use automatical RESTful mechanism to Update record
in your JS (example with JQuery) 
```Javascript
$.put('/@tasks/75',{title:"blah new"}); 
//you must use PUT method here
```
the result will be 
```json
{id:75, title:"blah new"}
```
you don't need to write server code, `any.php` does this for you.

### Example 7 : 
Use automatical RESTful mechanism with filter

if you want to do permision check upon auto RESTful request
you may just simply add delegate methods under 
```bash
${PROJECT_HOME}/delegate/RestfulDelegate.inc
```
in your JS (example with JQuery) 
```Javascript
$.put('/@tasks/75',{title:"blah new"});
//you must use PUT method here
```
in your /delegate/RestfulDelegate.inc
```php
<?php 
class RestfulDelegate{ 
    /** 
     * Delegate method for POST request on @tasks, 
     * you can also custom this in your conf/schemas/SCHEMA_NAME.ini 
     * with add [genderal] permission = F, or [genderal] restful = "get,put" 
     * @param params: http request params 
     */ 
    static function tasks_post($params){ 
         if(!isset($_SESSION["userId"])) //e.g. for login user only return false; 
            return true; 
    } 
}
```
if the is not logged in, then the result will be 
```json
{code:401, message:"RESTful ERROR : Sorry, You are not permited to do that."}
```


# DB 

## DB Schema (table definations) 
To use DB functions, You must tell `any.php` about your db schemas first. <br>
the schema files are under 
```bash
${PROJECT_HOME}/conf/schemas/*.ini 
```
Here is a very simple example. of file `/conf/schemas/tasks.ini`
```ini
[general] 
name = tasks	;table name = tasks 
pk = id	        ;primary key = id 
restful = "get,post" ;permit http get/post restful request. all : permit all ,none : disable auto restful 
permission = F	;allow the default auth group (guest) to access with 1111 permission. //1111 : the 1st 1 represents GET, the 2nd is POST, then PUT, DELETE. F means 'all' 

[schema] 
id = "BIGINT NOT NULL AUTO_INCREMENT" 
ownerId	= "BIGINT NOT NULL" 
title	= "varchar(256)"	

[index] 
ownerId = "normal"	;create index on ownerId
```
### NOTICE
* `[general]` > `name` must be same with the ini file name.
* If you changed the settings of the ini file after `migrate`, you have to affect the changes by writting SQL by yourself.

## DB Migration

After defined the schema files. You can create tables using migrate method via command line. 

```bash
${PROJECT_HOME}>php any.php migrate
# NOTICE: you have to create database and db user manually before running this command.
```

## DB Query

```php
/** 
 * if useCache == true * we will get result from Cache first.
 * with $sql as key * if there is no data in cache. the result from db will be add to cache * @see Use Cache for more information 
 */ 
$res = db_query($sql,$useCache=false); //$res is assoc-array
```

### DB Query With Condition
@see `DB.inc` for more information about conditions.<br>

### Syntax
```php
db_find(tablename, conditions); 
```
### example
```php
$res = db_find("tasks", [ "@fields" => "id", "regAt@>" => "2013-10-30", "title@?" => "Milk", ]); 
```
Equivalant SQL
```SQL
SELECT id FROM tasks WHERE regAt>'2013-10-30' AND title LIKE '%Milk%';
```


## DB Insert & Update
### Syntax
```php
db_save(schemaName, data, returnId=false); //save or update
```
### Example 1
```php
db_save('tasks',[title=>'My New Task'],true); 
```
Equivalant SQL
```SQL
INSERT INTO tasks (title,regAt) VALUES('My New Task',$now); 
```
Returns 
```json
{id:5,title:"My New Task",regAt:$now}
```

### Example 2
```php
db_save('tasks',[id=>3, title=>'New title']); 
```
Equivalant SQL
```SQL
UPDATE tasks SET title='New title' WHERE id=3;
```

## DB Import 
Insert multiple records to the same table to improve DB performance.
```php
db_import("tasks",[ [title=>"task1"], [title=>"task2",priority=>1], [title=>"task3"] ]); 
```
Equivalant SQL
```SQL
INSERT INTO tasks (title,priority,regAt) VALUES ("tasks1",0,$now),("tasks2",1,$now),("tasks3",0,$now);
```

## DB Transaction

### Example
insert into `tasks` table and get the last inserted id; <br>
if there are errors, `db->rollback` will be called automatically
```php
$res = db_trans([ 
  "insert into tasks (title) values ('my title')", 
  "select LAST_INSERT_ID() as 'last_id'" 
]); 
$id = $res[0]["last_id"]; 
```

# Cache
We use both APC & Memcached as our cache system

## GET/SET with cache functions
```php
/** 
 * @param key : key name
 * @param callback : a function returns value of $key, it is used when $key not exists in APC or Memcache 
 * @param sync: true means get from APC first then Memcache/Redis. false=APC only 
 */ 
function cache_get($key, $callback, $sync=true){} 

//set a cache by key value
function cache_set($key, $value, $time=3600, $sync=true){} 

//delete a cache by key name
function cache_del($key,$sync=true){}

//delete a cache
function mc_get($key, $callback){}

//get multiple keys
function mc_gets($keys){};

//delete a cache
function mc_set($key, $value, $time=3600){}

//delete a cache
function mc_del($key){}

/** @see Cache.inc for more informations*/
```
### Examples
```php
//if task in cache, then return. else fetch from db > return and add to cache.
$task = cache_get("task_$taskId",function() use($taskId){
    $task = db_find1st('tasks',[id=>$taskId]);
    return $task;
});
```


# HTML Template
`any.php` provides a lightway & very fast HTML render.<br>
There are only less than 10 kinds of keywords/syntax to remember, <br>
if you have the expierence of `Smarty` or other html templates, it will be very easy to start.

## How does it work

The HTML render compiles your template files to equivalent php file and cache them under 
```bash
${PROJECT_HOME}/tmp 
```

## The template file

### File Path
The template file for each `action` should be put over here.
```bash
/views/${CLIENT_TYPE}/${CONTROLLER_NAME}_${ACTION_NAME}.html
```

### $CLIENT_TYPE
depends where the request comes from. type could be one of the following types.  
there are mainly 5 types are supported

   * pc : request from a PC browser 
   * sm : request from smartphone, such as iPhone, iPod touch, Android 
   * pad : request from iPad or some brands of Android such like Galaxy... 
   * bot : a bot(spider) from google, Yahoo ... 
   * mail : html templates for emails

### The Layout File

You can specify a layout file to specify common HTML contents such like  `<headers>` or `<footers>`.<br>
The default path is
```bash
/views/${CLIENT_TYPE}/_layout.html 
```
the content of layout file should have a preserved keyword called `__CONTENTS__`.<br>
`__CONTENTS__` won't be replaced by template file of this `action`<br>
<br>
Here is the example of a layout file

```HTML
<!DOCTYPE html>
<html> 
  <head> 
    <title>evolany</title> 
    <meta http-equiv="X-UA-Compatible" content="IE=edge"> 
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
    <meta name="description" content=""> 
    <link rel="stylesheet" href="/css/my_css.css" type="text/css" /> 
    <script src="/js/my_js.js"></script> 
  </head> 
  <body> 
  __CONTENTS__ 
  </body> 
</html>
```
### Compiled php file path
```bash
/tmp/template-${DEVICE_TYPE}-${CONTROLLER_NAME}_${ACTION_NAME}.php
```

## Keywords & Syntax

Keywords/Syntax	Example	Equivalent PHP code	Description




| Syntax | Example | Equivalant PHP | Description |
| --- | --- | --- | --- |
| {$VAR_NAME} | {$myVar} | echo $myVar; | echo a php variable |
| {%KEY_NAME}	| {%myfile.mykey} | T("myfile.mykey"); | echo localized string |
| {var}	| {var $myvar=1} | $myvar=1; | declare a local php variable |
| {if}<br>{elseif}<br>{else} | {if $myvar>100}<br>&nbsp;&nbsp;larger than 100<br>{elseif $myvar<1}<br>&nbsp;&nbsp;less than 1 <br>{else}<br>&nbsp;&nbsp;others | if(\$myvar>100)<br>&nbsp;&nbsp;echo "larger than 100";<br>elseif($myvar<1)<br>&nbsp;&nbsp;echo "less than 1";<br>else<br>&nbsp;&nbsp;echo "others"; | if else ... clause |
| {for}<br>{break} | {for \$arr, \$item}<br>&nbsp;&nbsp;{$item.title}<br>{/for} | foreach(\$arr as \$item)<br>echo $item["title"]; | foreach<br>for ... break clause |
| {ignore} |\<script\><br>{ignore}<br>&nbsp;&nbsp;var myJSVar={id:"myId"};<br>{/ignore}<br>\<script\> | echo <<<EOF<br>\<script\><br>var myJSVar={id:"myId"};<br>\<script\><br>EOF; | ignore keywords |
| {include}	| {include 'myfile.html'} | | include other templates |


## Pass values to view templates

in your controller file 
```bash
/controllers/mycontroller.inc 
```
```php
function myaction($params){ 
    assign('myvar_1', 'val_1'); 
    render_html(false, [my_var2 => 'val_2']); 
    //false : means use default template name : mycontroller_myaction.html 
    //in this case 2 variables will be passed to view templates, myvar_1 & myval_2
}
```

in your template file 
```
/views/pc/mycontroller_myaction.html
```
add these variables to render
```html
<div>{$myvar_1}</div> <div>{$myvar_2}</div> 
```
then the result will be 
```html
<div>val_1</div> <div>val_2</div>
```

# Localization (Multi-lang)
`any.php` provides a very simple way to support multi-languages.<br>

in your text file 
```bash
/conf/text.csv
```
## Format of the CSV file
| id | en | jp | cn |
| --- | --- | --- | --- | 
| email | Email | メール | 邮件
| name | Name | 氏名 | 姓名

* the first line represents ID, and languages.
* ID is the key name to find the word. it should be an unique value.
  
## Specify user language
any.php will detect user's language automatically<br>
well you can also specify the default language by setting the value of `$_SESSION['lang']`
```php
$lang = $_SESSION['lang'] ?: (substr($_SERVER['HTTP_ACCEPT_LANGUAGE'], 0, 2) : Consts::$lang);
```

## Syntax
```php
$label = T($id,[$args]);
```
### PHP Examples
```php
$label = T('name'); 
```

### HTML Template Examples
```html
<label for='name'>{% name}</label><input name='name' type='text'/>
```

### Advanced Usage (variables)
`T()` function can also be used to replace variables<br>
in your csv
| id | en | jp | cn |
| --- | --- | --- | --- | 
| greeting_msg | Hi %s | %sさん、こんにちは！ | %s先生/女士, 你好！ |
```php
$msg = T('name', $_REQUEST['user_name']); 
//`%s` will be replaced with `user_name` from HTTP request parameters
```


# Filters

## What is Filter?
If you have the exp of playing with J2EE servlet. its easy to understand the concept of `Filter` .
* `Filter` aim to do common jobs `before` or `after` action function<br>
* `Filter` is a class contains `before` and `after` delegate methods<br>
* There can be more than 1 `Filter` exists, and each takes different duty.<br>
* Filter methods is executed sequencially, `before` filter is sequenced order, while `after` methods are executed reversely.
* Filter's `before`/`after` method can stop the http process at anytime.

Here is an example, in case of 2 filters exist.<br>
1 is UserFilter which handle user data, another is AuthFilter which handles Auth.
```
-> UserFilter->start()
-> AuthFilter->start()
-> CONTROLLER.inc :: ACTION($params)
-> AuthFilter->end()
-> UserFilter->end() 
```

## Example of AuthFilter
Here is an example of how do we use Filters to check Auth.

### Configuration
File path
```bash
${PROJECT_HOME}/conf/conf.inc
```
Set this property to Conf class
```php
static $filters	= ['auth'=>'/member'];
//Auth filter will triggered once user access anything under /member including subfolders
//You can also specify a filter upon all the requrests, to do this, just replace `/member` to `*`
```
Add a authfilter class
```bash
${PROJECT_HOME}/filters/auth.inc
```
```php
<?php 
class AuthFilter extends Filter{ 
    /** 
     * @param params : parameters of your http request. 
     * @return bool : false means stop HTTP request processing, and the action function wont be executed.
     */ 
    public function before($params){ 
        if(!($_SESSION['user_id'])){ 
            return false; 
        } 
        return true; 
    } 
}
```

# Permission & Auth Group
Permission
```
TODO
```


# OAuth
```
TODO
```

# Unit Test

## Test case file
Create test case file under `/test` with a name of 
```bash
/test/${YOUR_CONTROLLER}_${YOUR_ACTION}.csv
```
## Test case syntax
```csv
[method]	param_name_1     param_name_2
```
* [method] : Http methods of GET/POST/PUT/DELETE
* param_name_1... : acceptable parameter names of this action.

### Examples
The content of the csv file can look like this.

| [method] | age | name | 
| --- | --- | --- | 
| get | 18 | John |
| post | 20 | Siri |


## Run the test cases

From your browser , visit this url with no parameters
then you will find the result HTML or JSON
```bash
http://${YOUR_HOSTNAME}/${YOUR_CONTROLLER}/test_${YOUR_ACTION} 
```
