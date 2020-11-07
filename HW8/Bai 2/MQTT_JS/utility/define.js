var hostname = "localhost";
var port = 8000;
var clientId = makeid();
var path = "/mqtt";
var qos = 1;
var retain = false;
var tls = false;
var user = "";
var pass = "";
var keepAlive = 60;
var timeout = 3;
var cleanSession = false;

var Light1 = "/Light/One";
var Light2 = "/Light/Two";
var Light3 = "/Light/Three";
var Temp1  = "/Temp/One";
var Temp2  = "/Temp/Two";
var Temp3  = "/Temp/Three";

function makeid()
{
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    for( var i=0; i < 5; i++ )
        text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
}