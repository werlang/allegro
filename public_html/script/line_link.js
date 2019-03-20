var pre = $('pre#codigo').text();
//regex: qualquer_coisa\n
var lines = pre.match(/([\S\s]*?)\n/g);
if (lines==null){
	lines = new Array();
	$('pre#codigo li').each(function() {
		lines.push($(this).text()+"\n");
	})
}

$('span.linha').each( function(){
	var val = $(this).attr("data-ln");
	$(this).attr({"data-ln":lines[val-1]});
});

$(document).ready(function(){
	$('.linha').hover( function() {
		summon_tooltip($(this),"up",$(this).attr("data-ln"));
	});
	$(".linha").mouseout( function() {
		summon_tooltip();
	});
});

//multiplas linhas no codigo
//<pre class='n-linhas'>li-lf</pre>
$('pre.n-linhas').each( function() {
	var range = $(this).text().split("-");
	$(this).html("");
	range[0] = parseInt(range[0]);
	range[1] = parseInt(range[1]);
	for (var i=range[0] ; i<=range[1] ; i++){
		$(this).html($(this).html() + lines[i-1]);
	}
	$(this).attr({"class":"prettyprint linenums:"+range[0]});
});