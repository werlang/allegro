<!DOCTYPE html>
<head>
<meta charset='utf-8' />
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Aprendendo Allegro 5 - Janela</title>

<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
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
			<h1>Criando uma janela</h1>
			
			<p>A biblioteca Allegro possui uma série de tipos especiais de ponteiros e de funções. Seu funcionamento nos poupa bastante trabalho pois não precisamos entender como funciona a comunicação com o hardware.</p>
			
			<p>Vamos analisar abaixo um trecho de código. O detalhamento de seu funcionamento será explicado a seguir:</p>
			
				<pre class="prettyprint linenums" id='codigo'><?php printfile('ccode/tela.c'); ?></pre>
			
			<p>Na <span class='linha' data-ln='2'>linha 2</span> podemos ver o include na biblioteca principal do allegro. Nos próximos exemplos iremos fazer include em outras bibliotecas também. Isto se dá porque o allegro é dividido em diversas sub-bibliotecas, e dependendo dos módulos usados em cada programa, fazemos o include dos módulos necessários.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_DISPLAY'>ALLEGRO_DISPLAY</a></h2>
			<p>Na <span class='linha' data-ln='8'>linha 8</span> criamos um ponteiro do tipo ALLEGRO_DISPLAY. Este tipo de ponteiro serve para criar uma janela em nosso programa.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_init'>al_init()</a></h2>
			<p>Na <span class='linha' data-ln='11'>linha 11</span> chamamos a função al_init(), que serve para inicializar a biblioteca.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_create_display'>al_create_display()</a></h2>
			<p>Na <span class='linha' data-ln='14'>linha 14</span> chamamos a função al_create_display(), que serve para criar uma janela. Esta função recebe dois parâmetros que são a largura e altura da janela criada. E como ela retorna um ponteiro para a janela criada, atribuímos o retorno da função à variável janela que criamos antes.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_clear_to_color'>al_clear_to_color()</a></h2>
			<p>Na <span class='linha' data-ln='17'>linha 17</span> chamamos a função al_clear_to_color(), que preenche uma superfície com uma cor. Como não definimos nenhuma superfície (ainda), ele vai preencher a única existente, a janela. O parâmetro dela é uma cor (<a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_COLOR'>ALLEGRO_COLOR</a>).</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_map_rgb'>al_map_rgb()</a></h2>
			<p>A função al_map_rgb() recebe três números, compostos pelos pigmentos vermelho (R), verde (G) e azul (B). Cada um destes pigmentos vai de 0 a 255. A função retorna a cor gerada pela junção dos três pigmentos. Como usamos o retorno de al_map_rgb() como perâmetro de al_clear_to_color(), a cor gerada é preenchida na janela. Veja também a função <a target='_blank' href='https://www.allegro.cc/manual/5/al_map_rgba'>al_map_rgba()</a> para saber como criar cores com transparência.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_flip_display'>al_flip_display()</a></h2>
			<p>Na <span class='linha' data-ln='20'>linha 20</span> chamamos a função al_flip_display(), que faz com que o conteúdo de janela apareça na tela do computador. Note que até então só tínhamos criado o ponteiro janela na linha 8, criamos a janela (display) e dissemos que o ponteiro janela representará o display na linha 14. Na linha 17 preenchemos a janela com a cor branca (RGB 255,255,255). Mas em nenhum momento dissemos para a janela criada e colorida aparecer na tela. Isto é feito nessa linha 20, com o al_flip_display().</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_rest'>al_rest()</a></h2>
			<p>Na <span class='linha' data-ln='23'>linha 23</span>, usamos a função al_rest(), que faz com que o programa pare por um determinado tempo. Esta função recebe por parâmetro um valor numérico (double), que representa o número de segundos que o programa ficará parado. Portanto al_rest(10) faz com que o programa fiquei parado 10s enquanto al_rest(0.001) equivale a 1ms. Futuramente iremos abordar maneiras mais eficientes de controlar o tempo no allegro.</p>

			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_destroy_display'>al_destroy_display()</a></h2>
			<p>E finalmente, na <span class='linha' data-ln='26'>linha 26</span> chamamos a função al_destroy_display(), que recebe um ponteiro ALLEGRO_DISPLAY, e desaloca este display, liberando o ponteiro para outro uso.</p>
			
			<p>O resultado do programa acima será o surgimento de uma tela banca de 640x480px. Após 10 segundos, esta tela some e o programa encerra.</p>

			<div class='imagem'>
				<img src='img/tela.png' />
				<label>Janela em branco</label>
			</div>
			
			<h3>Outros links</h3>
			<ul>
				<li><a target='_blank' href='http://www.rafaeltoledo.net/tutorial-allegro-5-criando-uma-janela/'>Criando uma Janela</a></li>
				<li><a target='_blank' href='https://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial/Displays'>Allegro 5 Tutorial/Displays</a></li>
			</ul>
		</div>
		<div id='btn-footer'>
			<a class="button-red" href='index.php'>Anterior</a>
			<a class="button-red" href='imagem.php'>Próximo</a>
		</div>
	</div>
	
</div>
</body>
<script src="script/line_link.js"></script>
