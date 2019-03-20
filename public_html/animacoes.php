<!DOCTYPE html>
<head>
<meta charset='utf-8' />
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Aprendendo Allegro 5 - Animações</title>

<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>
<script src="script/popups.js"></script>
<script src="script/buttons.js"></script>
<link rel="stylesheet" href="css/stylesheet.css"/>
<link rel="stylesheet" href="css/popups.css"/>
<link rel='stylesheet' href='css/responsive.css'/> 
<link rel='stylesheet' href='css/buttons.css'/> 

</head>
<body>
<?php include_once("functions.php"); ?>
<?php include_once("side_menu.php"); ?>

<div class='page-wrapper'>
	<div class="container">
		<div class='col-m-12'>
			<h1>Criando animações</h1>
			
			<p>Agora que já sabemos usar timers, vamos ver como fazer para que um objeto se movimente em uma taxa pré-definida.</p>
			
			<p>Para isso, iremos utilizar o conceito de taxa de atualização de tela aprendido anteriormente. O objetivo desta lição é criar um quadrado vermelho, e fazer com que ele se movimente pela tela com uma velocidade fixa. Com isso, podemos demonstrar como criar animações sem que a velocidade destas sejam afetadas pelo desempenho do computador as executando.</p>

			<div class='imagem'>
				<img src='img/exemplo-animacoes.gif' >
				<label>Quadrado vermelho deslizando pela janela</label>
			</div>
			
			<pre class='prettyprint linenums' id='codigo'><?php printfile('ccode/animacoes.c'); ?></pre>
			
			<p>Na <span class='linha' data-ln='12'>linha 12</span> declaramos o bitmap que representará o quadrado vermelho que ficará se movendo na tela, e na <span class='linha' data-ln='42'>linha 42</span> criamos ele com as dimensões 50x50 pixels.</p>
			
			<p>Na <span class='linha' data-ln='49'>linha 49</span> mudamos o bitmap alvo para o <b>quadrado</b>, colorimos ele com a cor vermelha na <span class='linha' data-ln='50'>linha 50</span> e na <span class='linha' data-ln='51'>linha 51</span> mudamos o alvo de volta para o backbuffer da janela.</p>
						
			<p>O timer <b>timer</b>, que como explicado anteriormente serve para controlar a taxa de atualização da tela possui a frequencia de disparo de 60 Hz (60 vezes por segundo, ou 1/60s). Ele é criado na <span class='linha' data-ln='11'>linha 11</span>, seu tempo é definido na <span class='linha' data-ln='27'>linha 27</span>, ele é adicionado na fila de eventos na <span class='linha' data-ln='63'>linha 63</span> e iniciado na <span class='linha' data-ln='66'>linha 66</span></p>
			
			<h2>Lógica do programa:</h2>
			
			<pre class='n-linhas'>90-99</pre>
			<pre class='n-linhas'>105-116</pre>
			
			<ul>
				<li>Verifica se o timer foi disparado (<span class='linha' data-ln='90'>linha 90</span>).</li>
				<li>Incrementa a posição X (<span class='linha' data-ln='91'>linha 91</span>) e Y (<span class='linha' data-ln='92'>linha 92</span>) do quadrado.</li>
				<li>Verifica se posição X e Y do quadrado alcançou os limites da tela (linhas <span class='linha' data-ln='94'>94</span> e <span class='linha' data-ln='96'>96</span>). Em caso afirmativo inverte o sentido do movimento do respectivo eixo (+1 vira -1 e vice-versa) (linhas <span class='linha' data-ln='95'>95</span> e <span class='linha' data-ln='97'>97</span>). Assim o conteúdo de <b>dir_x</b> e <b>dir_y</b> irá somar 1 ou decrementar 1, dependendo do sentido desejado.</li>
				<li>Seta a flag <b>desenha=1</b> (<span class='linha' data-ln='98'>linha 98</span>) fazendo com que na etapa de atualização de tela, a mesma seja atualizada.</li>
				<li>Caso a tela precise ser atualizada (<b>desenha==1</b>) (<span class='linha' data-ln='105'>linha 105</span>), mostra o quadrado na posição designada (<span class='linha' data-ln='110'>linha 110</span>).</li>
			</ul>
			
			<h3>Outros links</h3>
			<ul>
				<li><a target='_blank' href='http://www.rafaeltoledo.net/tutorial-allegro-5-animacoes/'>Animações</a></li>
				<li><a target='_blank' href='http://www.rafaeltoledo.net/tutorial-allegro-5-12-ajustando-frames-por-segundo-fps/'>Ajustando Frames por Segundo FPS</a></li>
				<li><a target='_blank' href='https://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial/Bitmaps'>Allegro 5 Tutorial/Bitmaps</a></li>
				<li><a target='_blank' href='https://www.youtube.com/watch?v=YgDxmCt2ytg&list=PL9333715188CD7669&index=9'>Final Timed Game Loop in Allegro 5</a></li>
			</ul>
		</div>
		<div id='btn-footer'>
			<a class="button-red" href='timer.php'>Anterior</a>
			<a class="button-red" href='sprites.php'>Próximo</a>
		</div>
	</div>
</div>
</body>
<script src="script/line_link.js"></script>

