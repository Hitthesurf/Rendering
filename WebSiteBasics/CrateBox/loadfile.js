//Load a text resource from a file over the internet
var loadTextResource = function (url) {
	var request = new XMLHttpRequest();
	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
	request.onload = function () {console.log('Error: HTTP Status ' + request.status + ' on resource ' + url);};
	request.send();
};
