export default () => {
    const codeElement = document.querySelector('pre#codigo');
    if (!codeElement) return;
    var pre = codeElement.textContent;
    //regex: qualquer_coisa\n
    var lines = pre.match(/([\S\s]*?)\n/g);
    if (lines == null) {
        lines = [];
        document.querySelectorAll('pre#codigo li').forEach(function(li) {
            lines.push(li.textContent + "\n");
        });
    }
    document.querySelectorAll('span.linha').forEach(function(span) {
        var val = span.getAttribute("data-ln");
        span.setAttribute("data-ln", lines[val - 1]);
    });
    
    document.querySelectorAll('.linha').forEach(function(linha) {
        linha.addEventListener("mouseenter", function() {
            summon_tooltip(linha, "up", linha.getAttribute("data-ln"));
        });
        linha.addEventListener("mouseleave", function() {
            summon_tooltip();
        });
    });
    
    //multiplas linhas no codigo
    //<pre class='n-linhas'>li-lf</pre>
    document.querySelectorAll('pre.n-linhas').forEach(function(pre) {
        var range = pre.textContent.split("-");
        pre.innerHTML = "";
        range[0] = parseInt(range[0]);
        range[1] = parseInt(range[1]);
        for (var i = range[0]; i <= range[1]; i++) {
            pre.innerHTML += lines[i - 1];
        }
        pre.setAttribute("class", "prettyprint linenums:" + range[0]);
    });
}