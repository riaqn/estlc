var _MAGIC_AST_ =  '{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Abstraction", "value":"Abstraction", "arg":"filter", "type":{"id":"FuncType", "value":"Function Type<br>Int->sum@19093680->sum@19094072->sum@19094072", "left":"Int->Sum Type", "right":"Sum Type->Sum Type"}, "term":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Abstraction", "value":"Abstraction", "arg":"app", "type":{"id":"FuncType", "value":"Function Type<br>sum@19094072->sum@19094072->sum@19094072", "left":"Sum Type", "right":"Sum Type->Sum Type"}, "term":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Abstraction", "value":"Abstraction", "arg":"qsort", "type":{"id":"FuncType", "value":"Function Type<br>sum@19094072->sum@19094072", "left":"Sum Type", "right":"Sum Type"}, "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"l", "type":"Sum Type", "term":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Reference", "value" : "qsort<br>sum@19094072->sum@19094072" }, "arg":{ "id":"Reference", "value" : "l<br>sum@19094072" } } } }, "arg":{ "id":"Fixpoint", "value":"Fixpoint<br>sum@19094072->sum@19094072", "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"qsort", "type":{"id":"FuncType", "value":"Function Type<br>sum@19094072->sum@19094072", "left":"Sum Type", "right":"Sum Type"}, "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"l", "type":"Sum Type", "term":{ "id":"Desum", "value":"Desum", "sum":{ "id":"Reference", "value" : "l<br>sum@19094072" }, "cases":[{"id":"Case", "value":"Case<br>sum@19094072", "name":"nil_gxIX4a2dRE", "term":{ "id":"Reference", "value" : "l<br>sum@19094072" }}, {"id":"Case", "value":"Case<br>sum@19094072", "name":"cons_nat_bbSqWy6yhK", "term":{ "id":"Deproduct", "value":"Deproduct<br>sum@19094072", "product":{ "id":"Reference", "value" : "cons_nat_bbSqWy6yhK<br>product@19094736" }, "names":"x l0 ", "term":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Reference", "value" : "app<br>sum@19094072->sum@19094072->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Reference", "value" : "qsort<br>sum@19094072->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Reference", "value" : "filter<br>Int->sum@19093680->sum@19094072->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>Int->sum@19093680", "func":{ "id":"Reference", "value" : "<<br>Int->Int->sum@19093680" }, "arg":{ "id":"Reference", "value" : "x<br>Int" } } }, "arg":{ "id":"Reference", "value" : "l0<br>sum@19094072" } } } }, "arg":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Reference", "value" : "cons_natfrAQBc8Wsa<br>product@19094736->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>product@19094736", "func":{ "id":"Application", "value":"Application<br>sum@19094072->product@19094736", "func":{ "id":"Reference", "value" : "cons_nat<br>Int->sum@19094072->product@19094736" }, "arg":{ "id":"Reference", "value" : "x<br>Int" } }, "arg":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Reference", "value" : "qsort<br>sum@19094072->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Reference", "value" : "filter<br>Int->sum@19093680->sum@19094072->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>Int->sum@19093680", "func":{ "id":"Reference", "value" : ">=<br>Int->Int->sum@19093680" }, "arg":{ "id":"Reference", "value" : "x<br>Int" } } }, "arg":{ "id":"Reference", "value" : "l0<br>sum@19094072" } } } } } } }}]} } } } } }, "arg":{ "id":"Fixpoint", "value":"Fixpoint<br>sum@19094072->sum@19094072->sum@19094072", "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"app", "type":{"id":"FuncType", "value":"Function Type<br>sum@19094072->sum@19094072->sum@19094072", "left":"Sum Type", "right":"Sum Type->Sum Type"}, "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"l0", "type":"Sum Type", "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"l1", "type":"Sum Type", "term":{ "id":"Desum", "value":"Desum", "sum":{ "id":"Reference", "value" : "l0<br>sum@19094072" }, "cases":[{"id":"Case", "value":"Case<br>sum@19094072", "name":"nil_3soLBPh71Y", "term":{ "id":"Reference", "value" : "l1<br>sum@19094072" }}, {"id":"Case", "value":"Case<br>sum@19094072", "name":"cons_nat_exuieaoEiI", "term":{ "id":"Deproduct", "value":"Deproduct<br>sum@19094072", "product":{ "id":"Reference", "value" : "cons_nat_exuieaoEiI<br>product@19094736" }, "names":"x l2 ", "term":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Reference", "value" : "cons_natfrAQBc8Wsa<br>product@19094736->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>product@19094736", "func":{ "id":"Application", "value":"Application<br>sum@19094072->product@19094736", "func":{ "id":"Reference", "value" : "cons_nat<br>Int->sum@19094072->product@19094736" }, "arg":{ "id":"Reference", "value" : "x<br>Int" } }, "arg":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Reference", "value" : "app<br>sum@19094072->sum@19094072->sum@19094072" }, "arg":{ "id":"Reference", "value" : "l2<br>sum@19094072" } }, "arg":{ "id":"Reference", "value" : "l1<br>sum@19094072" } } } } }}]} } } } } } }, "arg":{ "id":"Fixpoint", "value":"Fixpoint<br>Int->sum@19093680->sum@19094072->sum@19094072", "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"filter", "type":{"id":"FuncType", "value":"Function Type<br>Int->sum@19093680->sum@19094072->sum@19094072", "left":"Int->Sum Type", "right":"Sum Type->Sum Type"}, "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"f", "type":{"id":"FuncType", "value":"Function Type<br>Int->sum@19093680", "left":"Int", "right":"Sum Type"}, "term":{ "id":"Abstraction", "value":"Abstraction", "arg":"l", "type":"Sum Type", "term":{ "id":"Desum", "value":"Desum", "sum":{ "id":"Reference", "value" : "l<br>sum@19094072" }, "cases":[{"id":"Case", "value":"Case<br>sum@19094072", "name":"nil_1xVPfvJcrg", "term":{ "id":"Reference", "value" : "l<br>sum@19094072" }}, {"id":"Case", "value":"Case<br>sum@19094072", "name":"cons_nat_RYwTiizs2t", "term":{ "id":"Deproduct", "value":"Deproduct<br>sum@19094072", "product":{ "id":"Reference", "value" : "cons_nat_RYwTiizs2t<br>product@19094736" }, "names":"x l0 ", "term":{ "id":"Desum", "value":"Desum", "sum":{ "id":"Application", "value":"Application<br>sum@19093680", "func":{ "id":"Reference", "value" : "f<br>Int->sum@19093680" }, "arg":{ "id":"Reference", "value" : "x<br>Int" } }, "cases":[{"id":"Case", "value":"Case<br>sum@19094072", "name":"true_rQF69AzBla", "term":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Reference", "value" : "cons_natfrAQBc8Wsa<br>product@19094736->sum@19094072" }, "arg":{ "id":"Application", "value":"Application<br>product@19094736", "func":{ "id":"Application", "value":"Application<br>sum@19094072->product@19094736", "func":{ "id":"Reference", "value" : "cons_nat<br>Int->sum@19094072->product@19094736" }, "arg":{ "id":"Reference", "value" : "x<br>Int" } }, "arg":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Reference", "value" : "filter<br>Int->sum@19093680->sum@19094072->sum@19094072" }, "arg":{ "id":"Reference", "value" : "f<br>Int->sum@19093680" } }, "arg":{ "id":"Reference", "value" : "l0<br>sum@19094072" } } } }}, {"id":"Case", "value":"Case<br>sum@19094072", "name":"false_x3CF3EDNhm", "term":{ "id":"Application", "value":"Application<br>sum@19094072", "func":{ "id":"Application", "value":"Application<br>sum@19094072->sum@19094072", "func":{ "id":"Reference", "value" : "filter<br>Int->sum@19093680->sum@19094072->sum@19094072" }, "arg":{ "id":"Reference", "value" : "f<br>Int->sum@19093680" } }, "arg":{ "id":"Reference", "value" : "l0<br>sum@19094072" } }}]} }}]} } } } } }';
var _MAGIC_TOKEN_ = '[{"type":"comment", "name" : "quicksort program in MyLang", "line" : 2}, {"type":"Type", "name" : "Type", "line" : 3}, {"type":"id", "name" : "list_nat", "line" : 3}, {"type":"=", "name" : "=", "line" : 3}, {"type":"|", "name" : "|", "line" : 4}, {"type":"nil", "name" : "nil", "line" : 4}, {"type":":", "name" : ":", "line" : 4}, {"type":"id", "name" : "list_nat", "line" : 4}, {"type":"|", "name" : "|", "line" : 5}, {"type":"id", "name" : "cons_nat", "line" : 5}, {"type":":", "name" : ":", "line" : 5}, {"type":"nat", "name" : "nat", "line" : 5}, {"type":"->", "name" : "->", "line" : 5}, {"type":"id", "name" : "list_nat", "line" : 5}, {"type":"->", "name" : "->", "line" : 5}, {"type":"id", "name" : "list_nat", "line" : 5}, {"type":"Func", "name" : "Func", "line" : 7}, {"type":"id", "name" : "filter", "line" : 7}, {"type":"(", "name" : "(", "line" : 7}, {"type":"id", "name" : "f", "line" : 7}, {"type":":", "name" : ":", "line" : 7}, {"type":"nat", "name" : "nat", "line" : 7}, {"type":"->", "name" : "->", "line" : 7}, {"type":"bool", "name" : "bool", "line" : 7}, {"type":")", "name" : ")", "line" : 7}, {"type":"(", "name" : "(", "line" : 7}, {"type":"id", "name" : "l", "line" : 7}, {"type":":", "name" : ":", "line" : 7}, {"type":"id", "name" : "list_nat", "line" : 7}, {"type":")", "name" : ")", "line" : 7}, {"type":":", "name" : ":", "line" : 7}, {"type":"id", "name" : "list_nat", "line" : 7}, {"type":"=", "name" : "=", "line" : 7}, {"type":"match", "name" : "match", "line" : 8}, {"type":"id", "name" : "l", "line" : 8}, {"type":"|", "name" : "|", "line" : 9}, {"type":"nil", "name" : "nil", "line" : 9}, {"type":"=>", "name" : "=>", "line" : 9}, {"type":"nil", "name" : "nil", "line" : 9}, {"type":"|", "name" : "|", "line" : 10}, {"type":"id", "name" : "cons_nat", "line" : 10}, {"type":"id", "name" : "x", "line" : 10}, {"type":"id", "name" : "l0", "line" : 10}, {"type":"=>", "name" : "=>", "line" : 10}, {"type":"match", "name" : "match", "line" : 10}, {"type":"(", "name" : "(", "line" : 10}, {"type":"id", "name" : "f", "line" : 10}, {"type":"id", "name" : "x", "line" : 10}, {"type":")", "name" : ")", "line" : 10}, {"type":"|", "name" : "|", "line" : 11}, {"type":"true", "name" : "true", "line" : 11}, {"type":"=>", "name" : "=>", "line" : 11}, {"type":"(", "name" : "(", "line" : 11}, {"type":"id", "name" : "cons", "line" : 11}, {"type":"id", "name" : "x", "line" : 11}, {"type":"(", "name" : "(", "line" : 11}, {"type":"id", "name" : "filter", "line" : 11}, {"type":"id", "name" : "f", "line" : 11}, {"type":"id", "name" : "l0", "line" : 11}, {"type":")", "name" : ")", "line" : 11}, {"type":")", "name" : ")", "line" : 11}, {"type":"|", "name" : "|", "line" : 12}, {"type":"false", "name" : "false", "line" : 12}, {"type":"=>", "name" : "=>", "line" : 12}, {"type":"(", "name" : "(", "line" : 12}, {"type":"id", "name" : "filter", "line" : 12}, {"type":"id", "name" : "f", "line" : 12}, {"type":"id", "name" : "l0", "line" : 12}, {"type":")", "name" : ")", "line" : 12}, {"type":"Func", "name" : "Func", "line" : 14}, {"type":"id", "name" : "app", "line" : 14}, {"type":"(", "name" : "(", "line" : 14}, {"type":"id", "name" : "l0", "line" : 14}, {"type":":", "name" : ":", "line" : 14}, {"type":"id", "name" : "list_nat", "line" : 14}, {"type":")", "name" : ")", "line" : 14}, {"type":"(", "name" : "(", "line" : 14}, {"type":"id", "name" : "l1", "line" : 14}, {"type":":", "name" : ":", "line" : 14}, {"type":"id", "name" : "list_nat", "line" : 14}, {"type":")", "name" : ")", "line" : 14}, {"type":":", "name" : ":", "line" : 14}, {"type":"id", "name" : "list_nat", "line" : 14}, {"type":"=", "name" : "=", "line" : 14}, {"type":"match", "name" : "match", "line" : 15}, {"type":"id", "name" : "l0", "line" : 15}, {"type":"|", "name" : "|", "line" : 16}, {"type":"nil", "name" : "nil", "line" : 16}, {"type":"=>", "name" : "=>", "line" : 16}, {"type":"id", "name" : "l1", "line" : 16}, {"type":"|", "name" : "|", "line" : 17}, {"type":"id", "name" : "cons", "line" : 17}, {"type":"id", "name" : "x", "line" : 17}, {"type":"id", "name" : "l2", "line" : 17}, {"type":"=>", "name" : "=>", "line" : 17}, {"type":"(", "name" : "(", "line" : 17}, {"type":"id", "name" : "cons", "line" : 17}, {"type":"id", "name" : "x", "line" : 17}, {"type":"(", "name" : "(", "line" : 17}, {"type":"id", "name" : "app", "line" : 17}, {"type":"id", "name" : "l2", "line" : 17}, {"type":"id", "name" : "l1", "line" : 17}, {"type":")", "name" : ")", "line" : 17}, {"type":")", "name" : ")", "line" : 17}, {"type":"Func", "name" : "Func", "line" : 19}, {"type":"id", "name" : "qsort", "line" : 19}, {"type":"(", "name" : "(", "line" : 19}, {"type":"id", "name" : "l", "line" : 19}, {"type":":", "name" : ":", "line" : 19}, {"type":"id", "name" : "list_nat", "line" : 19}, {"type":")", "name" : ")", "line" : 19}, {"type":":", "name" : ":", "line" : 19}, {"type":"id", "name" : "list_nat", "line" : 19}, {"type":"=", "name" : "=", "line" : 19}, {"type":"match", "name" : "match", "line" : 20}, {"type":"id", "name" : "l", "line" : 20}, {"type":"|", "name" : "|", "line" : 21}, {"type":"nil", "name" : "nil", "line" : 21}, {"type":"=>", "name" : "=>", "line" : 21}, {"type":"nil", "name" : "nil", "line" : 21}, {"type":"|", "name" : "|", "line" : 22}, {"type":"id", "name" : "cons", "line" : 22}, {"type":"id", "name" : "x", "line" : 22}, {"type":"id", "name" : "l0", "line" : 22}, {"type":"=>", "name" : "=>", "line" : 22}, {"type":"(", "name" : "(", "line" : 22}, {"type":"id", "name" : "app", "line" : 22}, {"type":"(", "name" : "(", "line" : 22}, {"type":"id", "name" : "qsort", "line" : 22}, {"type":"(", "name" : "(", "line" : 22}, {"type":"id", "name" : "filter", "line" : 22}, {"type":"(", "name" : "(", "line" : 22}, {"type":"<", "name" : "<", "line" : 22}, {"type":"id", "name" : "x", "line" : 22}, {"type":")", "name" : ")", "line" : 22}, {"type":"id", "name" : "l0", "line" : 22}, {"type":")", "name" : ")", "line" : 22}, {"type":")", "name" : ")", "line" : 22}, {"type":"(", "name" : "(", "line" : 23}, {"type":"id", "name" : "cons", "line" : 23}, {"type":"id", "name" : "x", "line" : 23}, {"type":"(", "name" : "(", "line" : 23}, {"type":"id", "name" : "qsort", "line" : 23}, {"type":"(", "name" : "(", "line" : 23}, {"type":"id", "name" : "filter", "line" : 23}, {"type":">=", "name" : ">=", "line" : 23}, {"type":"id", "name" : "x", "line" : 23}, {"type":")", "name" : ")", "line" : 23}, {"type":"id", "name" : "l0", "line" : 23}, {"type":")", "name" : ")", "line" : 23}, {"type":")", "name" : ")", "line" : 23}, {"type":")", "name" : ")", "line" : 23}, {"type":"(", "name" : "(", "line" : 25}, {"type":"id", "name" : "qsort", "line" : 25}, {"type":"(", "name" : "(", "line" : 25}, {"type":"id", "name" : "cons_nat", "line" : 25}, {"type":"int", "name" : "2", "line" : 25}, {"type":"(", "name" : "(", "line" : 25}, {"type":"id", "name" : "cons_nat", "line" : 25}, {"type":"int", "name" : "1", "line" : 25}, {"type":"(", "name" : "(", "line" : 25}, {"type":"id", "name" : "cons_nat", "line" : 25}, {"type":"int", "name" : "3", "line" : 25}, {"type":"nil", "name" : "nil", "line" : 25}, {"type":")", "name" : ")", "line" : 25}, {"type":")", "name" : ")", "line" : 25}, {"type":")", "name" : ")", "line" : 25}, {"type":")", "name" : ")", "line" : 25}]';
var ast, tokens;

function prepare_tokenstream(){
    tokens = $.parseJSON(_MAGIC_TOKEN_);
    var now = $("#initial-token-tuple");
    var now_element = now;
    var now_line = tokens[0]["line"];
    $.each(tokens, function(idx, tkn){
        if (tkn["line"] != now_line){
            var newline = $("#initial-token-newline").clone(true);
            newline.attr("id", "newline"+tkn["line"]);
            newline.css("display", "block");
            now_element.after(newline);
            now_element = newline;
            now_line = tkn["line"];
        }

        //alert(tkn["line"]);
        var new_token = now.clone(true);
        new_token.attr("id", "token"+idx.toString());
        new_token.children(".token-code").html(tkn["name"]);
        new_token.children(".token-type").html(tkn["type"]);
        
        new_token.css("display", "block");
        now_element.after(new_token);
        now = new_token;
        now_element = now;
        
    });
}

var nodeid;
function prepare_ast(node, father_element){
    //Object.prototype.toString.call(str) === "[object String]"
    
    var nownode = $("#initial-tree-node").clone(true);
    nownode.attr("id", "node" + nodeid.toString());
    nodeid++;

    /*
        nownode.children("a").html(node);
        nownode.css("display", "block");
        nownode.appendTo(father_element);
        nownode.children("ul").remove();
        
        nownode.children("a").css("background-color", "rgb(255, 201, 14)");
    */

    if (Object.prototype.toString.call(node) === "[object String]"){
        nownode.children("a").html(node);
        nownode.children("a").css("background-color", "rgb(255, 201, 14)");
        nownode.children("ul").remove();
    }else 
        nownode.children("a").html(node['value']);
    nownode.css("display", "block");
    nownode.appendTo(father_element);

    if (node['id'] == "Reference"){
        nownode.children("a").css("background-color", "rgb(255, 201, 14)");
        nownode.children("ul").remove();
    }
    
    if (node["id"] == "Abstraction"){
        //arg
            prepare_ast(node["arg"], nownode.children("ul"));
        //type
            prepare_ast(node["type"], nownode.children("ul"));
        //term
            prepare_ast(node["term"], nownode.children("ul"));
    }

    if (node["id"] == "Application"){
        //func
            prepare_ast(node["func"], nownode.children("ul"));
        //arg
            prepare_ast(node["arg"], nownode.children("ul"));
    }

    if (node["id"] == "Desum"){
        //sum
        prepare_ast(node["sum"], nownode.children("ul"));
        var caseList = node["cases"];
        var len = caseList.length;
        for(var i=0; i<len; i++){
            prepare_ast(caseList[i], nownode.children("ul"));            
        }
    } 
    if (node["id"] == "Case"){
        prepare_ast(node["name"], nownode.children("ul"));
        prepare_ast(node["term"], nownode.children("ul"));
    }

    if (node["id"] == "Deproduct"){
        prepare_ast(node["product"], nownode.children("ul"));        
        prepare_ast(node["names"], nownode.children("ul"));
        prepare_ast(node["term"], nownode.children("ul"));
    }

    if (node["id"] == "Fixpoint"){
        prepare_ast(node["term"], nownode.children("ul"));
    }

    if (node["id"] == "FuncType"){
        prepare_ast(node["left"], nownode.children("ul"));
        prepare_ast(node["right"], nownode.children("ul"));
    }
}

$(document).ready(function(){
    prepare_tokenstream();

    nodeid = 0;
    var root = ast = $.parseJSON(_MAGIC_AST_);
    //alert(root['id']);
    var root_element = $("#tree-root");
    prepare_ast(root, root_element);
    
    $("#token-tag").click();
});

$(".node").click(function(){
    var children = $(this).parent().children("ul");
    var prevPos = $(this).offset();
    
    //alert(children.css("display"));
    if (children.css("display").toString() == "block"){
        children.css("display", "none");
        $(this).css("background-color", "#5cb85c");
    } else if (children.css("display").toString() == "none"){
        children.css("display", "block");
        $(this).removeAttr("style");
    }

    var nextPos = $(this).offset();
    var delta = prevPos.left - nextPos.left;
    //alert(pos.left);
    //alert($(".tree-panel").scrollLeft());
    var nowLeft = $(".tree-panel").scrollLeft();
    $(".tree-panel").scrollLeft(nowLeft-delta);
    //$("html,body").animate({ scrollTop: pos.top-30 }, 500);
    //$("html,body").animate({ scrollLeft: pos.left-30 }, 500);

});


var token_display = true;
$("#token-tag").click(function(){
    //alert("HERE");
    token_display = !token_display;
    if (token_display){
        $("#token-panel").css("width", "50%");
        $("#token-tag").css("left", "50%");
        $("#token-tag").text("<<<<");
    } else{
        $("#token-panel").css("width", "0px");
        $("#token-tag").css("left", "0px");
        $("#token-tag").text(">>>>");
    }
});
