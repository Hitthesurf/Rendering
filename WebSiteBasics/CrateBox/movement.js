//Global Vars
var isDragging = false;
var dragStart = {x:0, y:0};
var theta = 0.0; //Radians
var phi = Math.PI/2-0.001; //Radians
var Orthographic = true;

var clamp = function(val, lower, upper)
{
    return Math.min(Math.max(lower,val),upper);  
}

var buttonClickProj = function(e)
{
    //e.relatedTarget.value = "Hello World";
    console.log(Orthographic);
    Orthographic = !Orthographic;
}

var onPointerDown = function(e)
{
    isDragging = true;
    dragStart.x = e.offsetX;
    dragStart.y = e.offsetY;
    console.log("Click down at " + dragStart.x + ", " + dragStart.y);
    
}

var onPointerUp = function(e)
{
    isDragging = false;
    console.log("Release Click at " + e.offsetX + ", " + e.offsetY);
}

var onPointerMove = function(e)
{
    if (isDragging)
    {
        theta = -(e.offsetX - dragStart.x)/100;
        phi = clamp(Math.PI/2 - 0.001+(e.offsetY - dragStart.y)/100,-Math.PI/2 + 0.001, Math.PI/2 - 0.001);
    }
}