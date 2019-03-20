<!DOCTYPE html>
<head>
<meta charset='utf-8' />
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Aprendendo Allegro 5 - Mouse</title>

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
			<h1>Utilizando a entrada do mouse</h1>
			
			<p>Neste tutorial iremos aprender a como utlilizar o mouse em nossos programas. A captura do movimento e cliques do mouse é feita através dos eventos, aprendidos na lição anterior. Neste exemplos iremos criar duas áreas coloridas, onde uma reagirá com o movimento do mouse, e a outra ao clique do mouse. Vamos ao código:</p>
			
			<pre class="prettyprint linenums" id='codigo'><?php printfile('ccode/mouse.c'); ?></pre>
			
			<p>No início do programa declaramos algumas variáveis, e iniciamos a biblioteca e addons. Tudo isso já foi visto anteriormente. Vamos direto ao que temos de novo.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_set_window_title'>al_set_window_title()</a></h2>
			<p>Na <span class='linha' data-ln='34'>linha 34</span> chamamos a função al_set_window_title(), que muda o texto da janela. Ela recebe o display, e a string contendo o texto.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_install_mouse'>al_install_mouse()</a></h2>
			<p>Na <span class='linha' data-ln='37'>linha 37</span> chamamos a função al_install_mouse(), permite o uso dos eventos relativos à captura de cliques e movimentos do mouse.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_set_system_mouse_cursor'>al_set_system_mouse_cursor()</a></h2>
			<p>Na <span class='linha' data-ln='44'>linha 44</span> chamamos a função al_install_mouse(), que que serve para definir o tipo de cursor do mouse que será usado em uma determinada janela. Ela recebe como parâmetro o display, e uma flag indicando o tipo de cursor. No exemplo, usamos ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT, que é o cursor padrão. Consulte a <a target='_blank' href='https://www.allegro.cc/manual/5/al_set_system_mouse_cursor'>referência</a> para conhecer os outros tipos de cursores disponíveis.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_create_bitmap'>al_create_bitmap()</a></h2>
			<p>Na <span class='linha' data-ln='51'>linha 51</span> e <span class='linha' data-ln='59'>linha 59</span> usamos a função al_create_bitmap(), que cria uma área gráfica, chamada bitmap, nas posições X e Y informadas por parâmetro. Já havíamos trabalhado com a função <a target='_blank' href='https://www.allegro.cc/manual/5/al_load_bitmap'>al_load_bitmap()</a>, que carrega uma imagem em um bitmap. A diferença é que al_create_bitmap() cria a área sem atribuir nada a ela, somente com as dimensões especificadas.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_get_mouse_event_source'>al_get_mouse_event_source()</a></h2>
			<p>Note que nas linhas seguintes criamos a fila de eventos (explicada anteriormente) e inicamos a mesma. Na <span class='linha' data-ln='75'>linha 75</span> porém, usamos como segundo parâmetro de <a target='_blank' href='https://www.allegro.cc/manual/5/al_register_event_source'>al_register_event_source()</a> (função que faz com que a fila de eventos guarde os eventos de uma determinada fonte de eventos) o retorno da função al_get_mouse_event_source(). Isto faz com que todos eventos relacionados ao mouse sejam registrados na fila de eventos. Diferente desta, lembre-se que na lição anterior havíamos registrado os eventos da janela na fila.</p>
			
			<h2>Lógica do problema</h2>

			<p>Os seguintes passos serão executados, até que a flag <b>sair==0</b>:</p>
			
			<ol>
				<li>Enquanto tiver eventos na fila:</li>
					<ol>
						<li>Verifica-se se o cursor do mouse está sobre o bitmap central. Se sim seta a flag <b>na_area_central=1</b>;</li>
						<li>Verifica-se se o clique do mouse foi feito sobre o bitmap menor. Se sim, seta a flag <b>sair=1</b>;</li>
					</ol>
				<li>Colore os dois bitmaps;</li>
				<li>Desenha um deles no centro da janela, e o outro no canto inferior direito;</li>
				<li>Atualiza a tela.</li>
			</ol>
			
			<p>Na <span class='linha' data-ln='78'>linha 78</span> criamos a flag <b>na_area_central</b> que começa em 0, e irá receber 1 quando o mouse estiver sobre o bitmap do centro da janela.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_is_event_queue_empty'>al_is_event_queue_empty()</a></h2>
			<p>Na <span class='linha' data-ln='81'>linha 81</span> usamos a função al_is_event_queue_empty que verifica se a fila de eventos passada por parâmetro está vazia. Como nossa while está condicionado à esta função, este trecho de código só deixará de ser executado quando todos os eventos tiverem sido tratados.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_wait_for_event'>al_wait_for_event()</a></h2>
			<p>Neste exemplo, usamos a função al_wait_for_event() na <span class='linha' data-ln='83'>linha 83</span>, que difere da função <a target='_blank' href='https://www.allegro.cc/manual/5/al_wait_for_event_until'>al_wait_for_event_until()</a> já utilizada porque ela não recebe o timeout como último parâmetro. Isto quer dizer que o programa fica esperando até que o evento aconteça.</p>
			
			<p>Na <span class='linha' data-ln='86'>linha 86</span> testamos se o tipo do evento disparado é ALLEGRO_EVENT_MOUSE_AXES, que representa o movimento do mouse.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_get_bitmap_width'>al_get_bitmap_width()</a> e <a target='_blank' href='https://www.allegro.cc/manual/5/al_get_bitmap_height'>al_get_bitmap_height()</a></h2>
			<p>O if das linhas <span class='linha' data-ln='88'>88</span> à <span class='linha' data-ln='91'>91</span> testa se o ponteiro do mouse está entre as dimensões do bitmap maior, nas coordenadas X e Y. a função al_get_bitmap_width() serve para pegar a largura do bitmap e al_get_bitmap_height() a altura. Os campos <b>mouse.x</b> e <b>mouse.y</b> da variável <a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_EVENT'>ALLEGRO_EVENT</a> acessam as posições X e Y do ponteiro do mouse, relativos à janela.</p>
			
			<p>Na <span class='linha' data-ln='99'>linha 99</span> testamos se o tipo do evento disparado é ALLEGRO_EVENT_MOUSE_BUTTON_UP, que representa o botão do mouse sendo soltado. Caso seja do desejo testar qual botão foi pressionado, usamos o campo <b>mouse.button</b> que possuirá os valores 1, 2 ou 3 para os botões esquerdo, direito e central, respectivamente. O if testa testa se o usuário ao clicar na janela, o cursor está dentro do espaço do bitmap menor. E em caso afirmativo seta a flag <b>sair=1</b>.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_set_target_bitmap'>al_set_target_bitmap()</a></h2>
			<p>Após sair da repetição que verifica os eventos, na <span class='linha' data-ln='113'>linha 113</span> chamamos a função al_set_target_bitmap() que muda o bitmap ativo para o bitmap <b>area_central</b>. Lembrando que as funções que fazem alterações em bitmaps (mudança de cor, desenho, etc) levam consideração sempre o bitmap ativo (por padrão o último display criado). Como já colorimos a janela com a cor preta na <span class='linha' data-ln='109'>linha 109</span> e agora queremos colorir o bitmap <b>area_central</b>, devemos mudar o bitmap ativo. Por isso chamamos a função al_set_target_bitmap(). Em seguida colorimos ele na cor branca (R:255 G:255 B:255) ou na cor verde (R:0 G:255 B:0), dependendo do valor da flag <b>na_area_central</b> (que depende da posição do cursor do mouse, conforme vimos acima, na <span class='linha' data-ln='92'>linha 92</span>). Em seguida o mesmo é feito com o outro bitmap, o <b>botao_sair</b>.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_get_backbuffer'>al_get_backbuffer()</a></h2>
			<p>Na <span class='linha' data-ln='126'>linha 126</span> queremos devolver o bitmap ativo para a janela, pois chegou o momento de escrever a <b>area_central</b> e <b>botao_sair</b> nela. Então chamamos a função al_set_target_bitmap(), mas ao invés de usar a janela como parâmetro, chamamos a função al_get_backbuffer(). E o porque disso será explicado.</p>
						
			<p>Existem duas áreas de desenho da tela: O front buffer, e o back buffer. Quando criamos o display, automaticamente ele se torna o display ativo (isso já sabemos), e todas rotinas de desenho são feitas no back buffer desse display. O que está no front buffer é o que é mostrado na tela. Pontanto, ao usar a função <a target='_blank' href='https://www.allegro.cc/manual/5/al_flip_display'>al_flip_display()</a>, o conteúdo do front e back buffer se inverte, fazendo com que mostremos o que antes estava no back buffer, por isso chamamos essa função de "função que atualiza a tela".</p>
			
			<p>Com isso, percebemos que o bitmat ativo (aquele que desenhamos em cima) nunca deve ser o front buffer do display. Por conta disso, que quando mudamos o bitmap ativo de volta para a janela, precisamos mudar para o back buffer da janela, daí entra a função al_get_backbuffer().</p>
			
			<p>Daí em diante no código é simples, desenhamos os dois bitmaps em posições diferentes da janela (no back buffer dela), e atualizamos a tela (trocando o front com o back buffer).</p>
					
			<div class='imagem'>
				<img src='img/exemplo-mouse.gif' >
				<label>Área central com e sem o cursor do mouse sobre</label>
			</div>

			<h3>Outros links</h3>
			<ul>
				<li><a target='_blank' href='http://www.rafaeltoledo.net/tutorial-allegro-5-5-utilizando-o-mouse/'>Utilizando o Mouse</a></li>
				<li><a target='_blank' href='https://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial/Input'>Allegro 5 Tutorial/Input</a></li>
				<li><a target='_blank' href='https://www.youtube.com/watch?v=7juhNYYjQWU&list=PL9333715188CD7669&index=7'>Mouse Input in Allegro 5</a></li>
			</ul>
		</div>
		<div id='btn-footer'>
			<a class="button-red" href='eventos.php'>Anterior</a>
			<a class="button-red" href='teclado.php'>Próximo</a>
		</div>
	</div>
	
</div>
</body>
<script src="script/line_link.js"></script>

<!--
regex:
<span class='tp' data-toggle="tooltip" title="<\?php printfile\('ccode\/mouse\.c',\d{1,3}\); \?>">([\s\S]*?)<\/span>
<span class='linha' data-ln='1'>$1</span>
-->