
A extremely simple &amp; fast php web framework

# Goals

### Extremely Fast
* Almost As fast as non-framework to handle JSON request.

### Extremely Easy to Use
* Only `1 file` with about 25 functions to remember.

# Contents

## Tutorial
* [Installation](#installation)
* [Hello world](#hello-world)

## Features
* [Structure](#structure)
* [Configurations](#configurations)
* [Controllers](#controllers)
* [URL Dispatching](#url-dispatching)
* [Database](#database)
* [Auto RESTful API](#auto-restful-api)
* [HTML Template](#html-template)
* [Cache](#cache)
* [Filters](#filters)
* [Permission](#permission)
* [Modules](#modules)
* [Delegates](#delegates)
* [Localization](#localization)
* [Auto Unit Test](#auto-unit-test)

## Tools
* [Mail](#Mail)
* [CSV](#CSV)
* [GD2 (Image processing)](#GD2)
* [S3](#S3)
* [DataSet](#DataSet)
* [PHPDoc](#PHPDoc)
* [SEO](#SEO)
* [Binary ](#Binary)

---


# Installation

## Requirement

* PHP 5.4 (with APC, GD2, Memcache(Memcached), PDO, pdo-mysql ...)
  or PHP 7.2+ with APCu ...
* Memcached

## Installation guide
Check out from here
```sh
cd YOUR_HTTP_DOCUMENT_ROOT 
git clone https://github.com/Evolany/any.php anyphp
```

# Hello world
Create project
```sh
cd YOUR_HTTP_DOCUMENT_ROOT/anyphp 
php ./liberphp.php create myproj 
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

---

# Structure

| Folder | Description |
| --- | --- |
| /conf | Configuration files |
| &nbsp;&nbsp;&nbsp;&nbsp;conf.inc | [Configuration](#configurations) file |
| &nbsp;&nbsp;&nbsp;&nbsp;/schema/*.ini | [DB Schema](#db-schema) files |
| &nbsp;&nbsp;&nbsp;&nbsp;text.csv | [Multi-lang text](#localization) file |
| /controllers | [Controller](#controllers) files |
| /delegate | [Delegate](#delegates) files |
| /filters | [Filter](#filters) files |
| /lib | 3rd party library files such as `aws.phar` |
| /modules | [Module](#modules) files |
| /script | Batch scripts files |
| /test | [Test cases CSV](#auto-unit-test) files |
| /tmp                | contains php files compiled from HTML Templates |
| /views              | [HTML view templates](#html-template) |
| /webroot            | a public folder for static contents |
| &nbsp;&nbsp;&nbsp;&nbsp;/css | css files |
| &nbsp;&nbsp;&nbsp;&nbsp;/font | ttf/otf font files |
| &nbsp;&nbsp;&nbsp;&nbsp;/html | .htm static html files<br>without php programs |
| &nbsp;&nbsp;&nbsp;&nbsp;/images | image files |
| &nbsp;&nbsp;&nbsp;&nbsp;/js | javascript files |
| /any.php | The gateway & framework file |


# Configurations
file path `conf/conf.inc`
```php
<?php
class Conf{	
  //available modes : Developing | Product | Maintenance ...
  static $mode 				= 'Developing';
  //default language settings, only works for T() function, @see Multi-lang
  static $lang 				= 'jp';
  //default controller file name belongs to folders under /controllers folder.
  static $default_controller	        = 'top'; 
  //default action function name
  static $default_action		= null;
  //custom path prefix of each request 
  static $path_prefix			= '';
  
  
  //MySQL settings
  static $db_engine			= 'mysql';
  static $db_host			= '127.0.0.1';
  static $db_port			= '3306';
  static $db_name			= 'mydb';
  static $db_user			= 'root';
  static $db_pass			= '123456';
  
  //Cross domain settings, to response requests from Widgets/<IFRAME>
  static $cross_domain_methods          = 'GET, POST, PUT, DELETE, OPTIONS';
  
  //Cache settings
  static $cache_hosts			= 'localhost';
  static $cache_port			= '11211';
  
  //Session Settings
  static $session_enable		= true;
  static $session_lifetime	        = 86400;
  
  //Filter settings
  static $filters		        = ['admin'=>'/^admin/','member'=>'/^member/'];
}

switch (Conf::$mode) {
  case 'Developing':
    Conf::$db_host		= '127.0.0.1';
    Conf::$db_user		= 'root';
    Conf::$db_pass		= 'root';
    Conf::$server_host 	        = 'http://MY-PROJECT.test';
    break;
  case 'Product':
    Conf::$db_host		= 'My_Prod_DB';
    Conf::$db_user		= 'MyDBUserName';
    Conf::$db_pass		= 'MyProdDBUserPass';
    break;
  case 'Maintenance':
    Conf::$filters		= ['maintenance'=>'*'];
    break;
  default:
    break;
}

```
## Settings for each environment
1. change the settings under `switch (Conf::$mode)` of `conf.inc`
2. change `Conf::$mode` from `Developing` to yours

# Controllers
A controller file is a PHP program to handle requests<br>
In `any.php` its under `/controllers` folder with file extention of `.inc` but not `.php`.<br>
### Example
The file name 
```bash
${PROJECT_HOME}/controllers/tasks.inc
```
```php
<?php

//default action , relative URL : GET /tasks
function get($q){
    //Understand user request
    //Do DB Access
    //Render JSON/HTML ...
}

//Another action , relative URL : /tasks/close
function close($q){
    //do tasks here
}

```
## Nested Path
Controllers can be put into different folders
```
Preparing
```

# URL dispatching
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


# Database

## DB Schema
To use DB functions, You must tell `any.php` about your db schemas (table structures) first. <br>
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

## Advanced Database Topics

### Multiple Databases
```
Preparing
```
### Table Connection
```
Preparing
```


# Auto RESTful API

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

# Permission

in `any.php`, permission is described as a N-bit binary number , like [Linux permissions](http://linuxcommand.org/lc3_lts0090.php)

## Linux Permission System Example
@see [Linux file system permissions]
```bash
> ls -l
-rw-r--r--@  1 username  staff   7571 Dec 10 15:09 TODO.md
# you can change the permission with
> chmod 755 TODO.md
# then the permission will look like 
-rwxr-xr-x@  1 username  staff   7571 Dec 10 15:09 TODO.md
```


## Permission & Auth Group

To specify the user groups, You need to implement this delegate method under
```bash
/delegate/AuthDelegate.inc
```
Here is the example, you have to return a `int` value between 0~N
```php
class AuthDelegate{
  /**
  * @return int: 0~N, which means current user's group ID
  */
  static function group(&$groups=null){
    if (session_status() == PHP_SESSION_ACTIVE){//using session
      if(!empty($_SESSION['user_id'])){
        return 1;
      }
      if(!empty($_SESSION['admin_id'])){
        return 2;
      }
    }
    return 0;
  }
}
```

## To set permissions

1) For Normal request:<br>
`controllers/YOUR_CONTROLLER.inc`
```php
/**
 * @permission = 8CF
 */
function get($q){...}
```

2) For Auto RESTful request:<br>
`conf/schemas/YOUR_SCHEMA.ini`
```ini
[general]
permisstion = 08F
```

meanings of each bit (08F)
* 1st : permission of guest group default=8 (read only)
* Nth : permission of Nth auth group (N = AuthDelegate::group()). default = F (all permission)

so the meaning of `08F` will be
* 1st=0 : No access for 1st group (default is GUEST)
* 2nd=8 : READ only access for 2nd group (default is USER)
* 3rd=F : ALL access for 3rd group (default is ADMIN)

Please `NOTICE` that, since all the permission data is cached in APCu, <br>
to affect the changes, you have to `restart` your Apache/Nginx server, to clear the cache.

## Available numbers of each bit(auth group)
| HEX | Binary | description |
| --- | --- | --- |
| 0 | 0000 | no permission for this group |
| 8 | 1000 | read only(GET) |
| 9 | 1001 | read(GET) + delete(DELETE) |
| A | 1010 | read(GET) + update(PUT) |
| B | 1011 | read(GET) + update(PUT), delete(DELETE) |
| C | 1100 | read(GET) + add(POST) |
| D | 1101 | read(GET) + add(POST), delete(DELETE) |
| E | 1110 | read(GET) + add(POST), update(PUT) |
| F | 1111 | read(GET) + add(POST), update(PUT), delete(DELETE) |


# Modules
To separate Model (DB/file/remote URL access) from view controllers<br>
we suggest to extract business logics to Module files.<br>
these `Module` files can be found under

```bash
${PROJECT_HOME}/modules/*.inc
```

for example, to use all features of FB, we put all methods into 
```bash
${PROJECT_HOME}/modules/Fb.inc
```
and to use a method, we can just call
```php
//to get settings
$settings = Fb::getSettings($token, 'menu');
```

`NOTICE` : <br>
* `Module` filename has to be `capitalized`. with an extenstion of `.inc`
* All public methods should be `static` instead of `instance method`
* `DO NOT` need to `#include` the module file, `any.php` will do that for you.


# Delegates
```
Preparing
```

# Localization
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


# Auto Unit Test

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



# OAuth
```
TODO
```


# Tools

## Mail
```php
//syntax
Email::sendHTML($from,$to,$template,$data=[],$title=false);

//example
Email::sendHTML('no-reply@anybot.me', 'username@gmail.com', 'register', 
    ['pass'=>keygen(6), 'email' => 'username@gmail.com'], 'Welcome to Anybot');
```

your template file
```bash
#common layout
${PROJECT_HOME}/views/mail/_layout.html
#mail body
${PROJECT_HOME}/views/mail/register.html
```

An example of the mail template file, @see [HTML Template](#html-template)
```html
<!-- register.html -->
<fieldset>
  <legend>Your account info : </legend>
  <ul>
    <li>Email : <b>{$email}</b></li>
    <li>Password : <b>{$pass}</b></li>
  </ul>
</fieldset>
```


## CSV

### CSV Import
```php
/** Syntax
 * @param file : local or remote filename
 * @param table : db table(schema) name which can be found under conf/schemas/*.ini
 * @param force : ignore errors
 * @param encode : SJIS/UTF-8 ...
 * @param max_rows : 0 means no limitaion
 * @param row_parser : a function with input of raw data of each row, and output an array for import.
 */
Csv::import($file, $table, $force=true, $encode='SJIS', $max_rows=0, $row_parser);

```

### CSV Export
```php
/** Syntax
 * @param file : the filename to save
 * @param titles : an array contains titles of the 1st row
 * @param datas : csv data list
 * @param encode : SJIS/UTF-8 ...
 */
Csv::export($filename, $titles, $datas, $encode='SJIS');

```

## S3
```php

/**
* upload a file to S3
* @param [type] $fn : local file name
* @param [type] $s3fn : s3 file name
* @return void
*/
Amazon::s3upload($fn,$s3fn,$meta=[]);
    
/**
* write data to a file in S3
* @param [type] $body : file body
* @param [type] $s3fn : s3 file name
* @return void
*/
Amazon::s3write($body,$s3fn);

/**
* download a file from S3
* @param [type] $bucket
* @param [type] $s3fn : s3 file name
* @param [type] $fn : donwload file name
* @return void
*/
Amazon::s3download($bucket,$s3fn,$fn);

/**
* Remove a file from S3
* @param [type] $bucket
* @param [type] $s3fns : array of filename or filename string
* @return void
*/
Amazon::s3remove($bucket,$s3fns);
}
```

## GD2
Image manipulation / combine / editing / croping ... <br>
@see [GD.inc](https://github.com/Evolany/any.php/blob/master/modules/utils/GD.inc)

## DataSet
```
Preparing
```

## PHPDoc
```
Preparing
```

## SEO
```
Preparing
```

## Binary 
```
Preparing
```
