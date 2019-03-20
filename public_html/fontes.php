<!DOCTYPE html>
<head>
<meta charset='utf-8' />
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Aprendendo Allegro 5 - Fontes e texto</title>

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
			<h1>Utilizando fontes e saída de texto</h1>
			
			<p>Neste tutorial vamos tratar sobre como mostrar na tela mensagens de texto. Para isso, iremos também aprender a carregar arquivos de fonte. Neste exemplo iremos mostrar na tela três mensagens de texto, conforme a imagem abaixo.</p>
						
			<div class='imagem'>
				<img src='img/texto.png' >
				<label>Saída de texto do allegro</label>
			</div>

			<pre class="prettyprint linenums" id='codigo'><?php printfile('ccode/fontes.c'); ?></pre>
			
			<p>Nas linhas <span class='linha' data-ln='4'>4</span> e <span class='linha' data-ln='5'>5</span> fazemos um include nas bibliotecas dos addons allegro_font e allegro_ttf, que possuem as funções que permitem trabalhar com as fontes.</p>
			
			<pre class='n-linhas'>13-17</pre>
			
			<p>Na <span class='linha' data-ln='13'>linha 13</span> criamos a função <b>error_msg</b> para facilitar a tarefa de mostrar mensagens de erro. Como pode-se notar mais a frente no código, neste tutorial estamos verificando se cada chamada de função apresentou erro (retornou NULL) ou não:</p>

			<p>Na inicialização do allegro</p>			
			<pre class='n-linhas'>26-30</pre>
			
			<p>Addon de fontes ttf</p>
			<pre class='n-linhas'>35-39</pre>

			<p>Criação do display</p>
			<pre class='n-linhas'>41-46</pre>

			<p>Carregamento da fonte</p>
			<pre class='n-linhas'>48-54</pre>
			
			Em caso de erro, mostramos uma mensagem com o <a target='_blank' href='https://www.allegro.cc/manual/5/al_show_native_message_box'>al_show_native_message_box</a> e encerramos o programa com <b>return -1</b>.</p>

			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_FONT'>ALLEGRO_FONT</a></h2>
			<p>Na <span class='linha' data-ln='24'>linha 24</span> declaramos nossa variável que irá receber a fonte que será usada ao mostrar textos na tela.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_init_font_addon'>al_init_font_addon()</a> e <a target='_blank' href='https://www.allegro.cc/manual/5/al_init_ttf_addon'>al_init_ttf_addon()</a></h2>
			<p>Na <span class='linha' data-ln='33'>linha 33</span> iniciamos o addon de fontes do allegro, e na <span class='linha' data-ln='36'>linha 36</span> o addon de fontes ttf (True Type Font). Este segundo serve para o uso de fontes arquivos de fontes .ttf.</p>

			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_load_font'>al_load_font()</a></h2>
			<p>Na <span class='linha' data-ln='49'>linha 49</span> chamamos a função al_load_font(), que serve para carregar uma fonte de um arquivo para ser usada como fonte no programa. A função recebe uma strings, contendo o caminho do arquivo (ou nome, se estiver na mesma pasta), o tamanho da fonte, em pixels, e o terceiro parâmetro são flags de renderização da fonte,0 como padrão (consulte <a target='_blank' href='https://www.allegro.cc/manual/5/al_load_ttf_font'>referência</a> para mais detahes).</p>

			<p>O programa então pinta a janela com a cor branca na <span class='linha' data-ln='57'>linha 57</span>.</p>

			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_draw_text'>al_draw_text()</a></h2>
			<p>Nas linhas <span class='linha' data-ln='60'>60</span>, <span class='linha' data-ln='63'>63</span> e <span class='linha' data-ln='66'>66</span> chamamos a função al_draw_text(), que serve para escrever um texto na tela. A função recebe como parâmetro a fonte (ALLEGRO_FONT), a cor do texto (por isso usamos a função <a target='_blank' href='https://www.allegro.cc/manual/5/al_map_rgb'>al_map_rgb()</a> para montar a cor), as coordenadas X e Y onde o texto aparecerá, a flag de alinhamento horizontal do texto, e a string contendo o texto. Os possíveis valores para a flag de alinhamento horizontal são os seguintes:</p>
			
			<ul>
				<li>ALLEGRO_ALIGN_LEFT - Posição X informada corresponde ao canto esquerdo do texto.</li>
				<li>ALLEGRO_ALIGN_CENTRE - Centro do texto localizado na posição X informada.</li>
				<li>ALLEGRO_ALIGN_RIGHT - Posição X informada corresponde ao canto direito do texto.</li>
			</ul>

			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_draw_textf'>al_draw_textf()</a></h2>
			<p>Na <span class='linha' data-ln='71'>linha 71</span> usamos a função al_draw_textf(), que funciona de maneira semelhante à al_draw_text(). A diferença é que ela recebe um texto formatado, como a conhecida função printf().</p>

			<p>Após isso, atualizamos a tela (<span class='linha' data-ln='74'>linha 74</span>), paramos o programa por 10 segundos (<span class='linha' data-ln='77'>linha 77</span>) e desalocamos a fonte (<span class='linha' data-ln='80'>linha 80</span>) e janela (<span class='linha' data-ln='81'>linha 81</span>). O resultado é um programa que irá mostrar as 4 mensagens de texto na tela por 10 segundos.</p>
			
			<h3>Download dos arquivos</h3>
			<ul>
				<li><a href='ccode/arial.ttf' download>Fonte do texto</a></li>
			</ul>

			<h3>Outros links</h3>
			<ul>
				<li><a target='_blank' href='http://www.rafaeltoledo.net/tutorial-allegro-5-4-escrevendo-texto-usando-fontes-true-type-ttf/'>Escrevendo Texto Usando Fontes True Type (TTF)</a></li>
				<li><a target='_blank' href='https://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial/Addons/Fonts'>Allegro 5 Tutorial/Addons/Fonts</a></li>
				<li><a target='_blank' href='https://www.youtube.com/watch?v=QxZnq6wN6V8&list=PL9333715188CD7669&index=3'>Using Fonts and Text in Allegro 5</a></li>
			</ul>
		</div>
		<div id='btn-footer'>
			<a class="button-red" href='mensagem.php'>Anterior</a>
			<a class="button-red" href='eventos.php'>Próximo</a>
		</div>
	</div>
	
</div>
</body>
<script src="script/line_link.js"></script>
