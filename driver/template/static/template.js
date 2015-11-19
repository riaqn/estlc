$(document).ready(function(){
    
});

$(".node").click(function(){
    var children = $(this).parent().children("ul");
    
    //alert(children.css("display"));
    if (children.css("display").toString() == "block"){
        children.css("display", "none");
        $(this).css("background-color", "#5cb85c");
    } else if (children.css("display").toString() == "none"){
        children.css("display", "block");
        $(this).removeAttr("style");
    }

});