$(document).ready(function() {
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ".split("")
    cell_expressions = {}

    function MyLength(a) { return a.length; }
    function EvalExpression(expression, cellId) {
        if (expression[0] == "=") {
            expression = expression.slice(1);
            expression = expression.split("ABS").join("Math.abs");
            expression = expression.split("SIN").join("Math.sin");
            expression = expression.split("LEN").join("MyLength");
            for(var i = 0, size = alphabet.length; i < size; i++) {
                var letter = alphabet[i];
                for (j = 20; j >= 0; j--) {
                    var cellIdToBeFound = letter + j.toString();
                    if (expression.indexOf(cellIdToBeFound) !== -1) {
                        var val = EvalExpression(cell_expressions[cellIdToBeFound]);
                        expression = expression.split(cellIdToBeFound).join(val.toString());
                    }
                }
            }
            try {
                var lenSplitParts = expression.split("MyLength(");
                for (var i = 1, size = lenSplitParts.length; i < size; i++) {
                    if (lenSplitParts[i][0] != "\"" && lenSplitParts[i][0] != "'") {
                        var bracketSplit = lenSplitParts[i].split(")");
                        bracketSplit[0] = '"' + bracketSplit[0] + '"';
                        lenSplitParts[i] = bracketSplit.join(")");
                    }
                }
                expression = lenSplitParts.join("MyLength(");
                expression = eval(expression);
            } catch(e) {
                console.log("Bad formula at " + cellId)
                expression = "";
                throw "Bad formula at " + cellId
            }
        }
        return expression;
    }

    for(var i = 0, size = alphabet.length; i < size; i++){
        var letter = alphabet[i];
        for (j = 1; j < 21; j++) {
            var cellId = letter + j.toString();
            cell_expressions[cellId] = "";
            $('#' + cellId).val("");
            $('#' + cellId).focusin(function() { 
                                    $(this).val(cell_expressions[$(this).attr('id')]); 
                                });
            $('#' + cellId).focusout(function() { 
                                    cell_expressions[$(this).attr('id')] = $(this).val();
                                    var evalRes = "";
                                    try {
                                        evalRes = EvalExpression($(this).val(), $(this).attr('id'));
                                    } catch(e) {
                                    }
                                    $(this).val(evalRes); 
                                });
        }
    } 
});