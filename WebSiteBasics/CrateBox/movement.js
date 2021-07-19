//Global Vars
var isDragging = false;
var dragStart = {x:0, y:0};
var theta = 0.0; //Radians
var phi = 0.0; //Radians

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
        phi = (e.offsetY - dragStart.y)/100;
    }
}