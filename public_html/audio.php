<!DOCTYPE html>
<head>
<meta charset='utf-8' />
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Aprendendo Allegro 5 - Audio</title>

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
			<h1>Manipulando audio</h1>
			
			<p>Nesta lição iremos aprender como carregar arquivos de audio para nossos programas. As utilidades vão desde adicionar músicas de fundo até utilizar efeitos sonoros sob determinadas condições do programa.</p>
			
			<p>No programa em questão, colocaremos na janela dois retângulos, um azul e um vermelho. Quando o usuário clica um deles um efeito sonoro diferente para cada um é reproduzido. Enquanto isso, uma música de fundo é tocada em repetição.</p>
						
			<div class='imagem'>
				<img src='img/audio.png' >
				<label>Retângulos esperando serem clicados</label>
			</div>

			<div class='imagem'>
				<audio controls>
				  <source src="ccode/soundtrack.ogg" type="audio/ogg">
				Your browser does not support the audio element.
				</audio>
				<label>Música de fundo</label>
			</div>

			<div class='imagem'>
				<audio controls>
				  <source src="ccode/wololo.ogg" type="audio/ogg">
				Your browser does not support the audio element.
				</audio>
				<label>Wololo. Quando clica no retângulo vermelho</label>
			</div>

			<div class='imagem'>
				<audio controls>
				  <source src="ccode/aiao.ogg" type="audio/ogg">
				Your browser does not support the audio element.
				</audio>
				<label>Aiao. Quando clica no retângulo azul</label>
			</div>
			
			<pre class='prettyprint linenums' id='codigo'><?php printfile('ccode/audio.c'); ?></pre>
								
			<p>Nas linhas <span class='linha' data-ln='3'>3</span> e <span class='linha' data-ln='4'>4</span> fazemos include nas bibliotecasdos addons allegro_audio e allegro_acodec, que permitem que utilizemos as funções de audio do allegro.</p>
			
			<p>Antes de detalhar o código, vou explicar um pouco sobre a estrutura do addon de audio do allegro.</p>
			
			<p>O som que ouvimos é o produto de uma mistura de 1 ou mais componentes sonoros. Existem dois tipos de componentes sonores, o <a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_SAMPLE'>ALLEGRO_SAMPLE</a> (amostra de som), e o <a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_AUDIO_STREAM'>ALLEGRO_AUDIO_STREAM</a> (faixa de audio).</p>
			
			<p>O sample é o tipo de som utilizado para produzir efeitos sonoros rápidos, e seu carregamento é feito de uma vez só, O audio stream é utilizado para guardar sons mais longos, como por exemplo músicas de fundo, e o carregamento do conteúdo da faixa é feita aos poucos do arquivo, na medida do necessário. Isto é interessante pois não ocupa desnecessariamente a memória.</p>
			
			<p>Os samples e audio stream são misturados em um mixer do tipo <a target='_blank' href='https://www.allegro.cc/manual/5/allegro_mixer'>ALLEGRO_MIXER</a>, e o produto deste é um buffer com parâmetros como frequencia, volume, etc. ao longo do tempo que então é jogado para o hardware de saída e podemos ouvir.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_SAMPLE'>ALLEGRO_SAMPLE</a></h2>
			<p>Na <span class='linha' data-ln='11'>linha 11</span> e <span class='linha' data-ln='12'>linha 12</span> criamos variáveis do tipo ALLEGRO_SAMPLE. Este tipo de variável serve para guardar um efeito sonoro. Estes dois samples serão rproduzidos quando o usuário clicar nos retângulos azui e vermelho.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_AUDIO_STREAM'>ALLEGRO_AUDIO_STREAM</a></h2>
			<p>Na <span class='linha' data-ln='14'>linha 14</span></p> criamos uma variável do tipo ALLEGRO_AUDIO_STREAM, que receberá a música de fundo tocada.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_install_audio'>al_install_audio()</a> e <a target='_blank' href='https://www.allegro.cc/manual/5/al_init_acodec_addon'>al_init_acodec_addon()</a></h2>
			<p>Na <span class='linha' data-ln='32'>linha 32</span> iniciamos o addon de audio do allegro, e na <span class='linha' data-ln='38'>linha 38</span> o addon de codecs que permite que o allegro trabalhe com diferentes tipos de arquivos de audio</p>

			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_reserve_samples'>al_reserve_samples()</a></h2>
			<p>Na <span class='linha' data-ln='44'>linha 44</span> chamamos a função al_reserve_samples() que recebe um número como parâmetro, e tenta reservar na memória espaço suficiente para guardar este número de samples. Ela também automaticamente cria um novo mixer (ver <a target='_blank' href='https://www.allegro.cc/manual/5/allegro_mixer'>ALLEGRO_MIXER</a>), anexa estes samples a ele, e o torna o mixer padrão. Note que o número passado por parâmetro será o número máximo de samples simultâneos que poderão ser reproduzidos.</p> 
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_load_sample'>al_load_sample()</a></h2>
			<p>Na <span class='linha' data-ln='50'>linha 50</span> e <span class='linha' data-ln='55'>linha 55</span> carregamos o sample do arquivo passado por parâmetro da função al_load_sample() e o atribuímos às variáveis sample <b>som_verm</b> e <b>som_azul</b>, respectivamente.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_load_audio_stream'>al_load_audio_stream()</a></h2>
			<p>Na <span class='linha' data-ln='63'>linha 63</span> carregamos a faixa de audio do arquivo passado por parâmetro da função al_load_audio_stream() e o atribuímos à variável audio stream <b>musica</b>. O segundo e terceiro parâmetros da função são buffers e samples do stream. Há pouca documentação sobre o uso destes dois parâmetros, e todos exemplos que encontrei usam os valores <b>4</b> e <b>1024</b> para os dois, então usaremos estes mesmo valores.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_attach_audio_stream_to_mixer'>al_attach_audio_stream_to_mixer()</a></h2>
			<p>Na <span class='linha' data-ln='72'>linha 72</span> chamamos a função al_attach_audio_stream_to_mixer() que faz a ligação entre o audio stream e o mixer. Como parâmetro ele recebe o stream que queremos adicionar, e o mixer. Como não declaramos nenhuma variável do tipo ALLEGRO_MIXER, e estamos usando o mixer padrão criado pela função al_reserve_samples(), chamamos a função <a target='_blank' href='https://www.allegro.cc/manual/5/al_get_default_mixer'>al_get_default_mixer()</a>, que retorna o mixer padrão, para usar como parâmetro.</p> 
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_set_audio_stream_playmode'>al_set_audio_stream_playmode()</a></h2>
			<p>Na <span class='linha' data-ln='74'>linha 74</span> chamamos a função al_set_audio_stream_playmode() determina o modo de reprodução do stream passado no primeiro parâmetro. Este modo de reprodução é uma flag do tipo <a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_PLAYMODE'>ALLEGRO_PLAYMODE</a>, e os valores mais comuns de uso são:</p>
			
			<ul>
				<li>ALLEGRO_PLAYMODE_ONCE - Reproduz a stream/sample uma única vez</li>
				<li>ALLEGRO_PLAYMODE_LOOP - Reproduz a stream/sample em modo de repetição</li>
			</ul>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_set_audio_stream_playing'>al_set_audio_stream_playing()</a></h2>
			<p>Caso seja do desejo interromper ou continuar a reprodução de um stream, podemos usar a função al_set_audio_stream_playing(), que recebe por parâmetro a variável ALLEGRO_AUDIO_STREAM e um valor, sendo que 0/false representa que a stream deverá parar sua reprodução e 1/true que deverá continuar/começar a reprodução.</p>			
			<h2>Lógica do programa</h2>
			<p>Na <span class='linha' data-ln='160'>linha 160</span> testamos se existe um evento de movimento de mouse na lista de eventos, e então verificamos se as posições X e Y do mouse estão nos limites da área de um dos dois retângulos. Caso o mouse esteja dentro da área do retângulo azul (linhas <span class='linha' data-ln='162'>162</span> e <span class='linha' data-ln='164'>164</span>) atribuímos o valor <b>area=1</b>, e caso esteja dentro da área do triângulo vermelho (linhas <span class='linha' data-ln='162'>162</span> e <span class='linha' data-ln='167'>167</span>) atribuímos o valor <b>area=2</b>. Por fim, caso não esteja dentro da área de nenhum dos dois, <b>area=0</b>. Desta maneira, quando o clique do botão do mouse é detectado na <span class='linha' data-ln='175'>linha 175</span>, varificamos o valor de <b>area</b> para determinar em qual das duas áreas ele foi clicado.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_play_sample'>al_play_sample()</a></h2>
			<p>Nas linhas <span class='linha' data-ln='178'>178</span> e <span class='linha' data-ln='182'>182</span> chamamos a função al_play_sample() que serve para reproduzir um sample. Ela recebe os seguintes parâmetros, em ordem:</p>

			<ul>
				<li>Sample: A variável ALLEGRO_SAMPLE que será reproduzida.</li>
				<li>Ganho: Volume da sample, onde 1.0 é o volume original.</li>
				<li>Balanço: Distribuição do som no ambiente, onde -1.0 é lado esquerdo, 1.0 lado direito e 0.0 o centro.</li>
				<li>Velocidade: Velocidade que o som é reproduzido, onde 1.0 significa a velocidade original.</li>
				<li>Loop: Modo de reprodução (<a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_PLAYMODE'>ALLEGRO_PLAYMODE</a>). Veja acima para saber sobre os valores.</li>
				<li>Id: Ponteiro para uma variável que receberá o id da sample sendo tocada, ou NULL caso não necessite desta informação.</li>
			</ul>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_stop_sample'>al_stop_sample()</a></h2>
			<p>Caso seja do interesse interromper a reprodução de uma sample, chamamos a função al_stop_sample(), que recebe como parâmetro um ponteiro para a id da sample sendo reproduzida (variável tipo <a target='_blank' href='https://www.allegro.cc/manual/5/ALLEGRO_SAMPLE_ID'>ALLEGRO_SAMPLE_ID</a>), obtida através do último parâmetro da função al_play_sample().</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_set_audio_stream_gain'>al_set_audio_stream_gain()</a></h2>
			<p>A função al_set_audio_stream_gain() serve para determinar o volume de uma faixa de audio. Ela recebe uma faixa de audio como parâmetro, e um valor float, onde 1.0 significa volume original.</p>

			<h3>Download dos arquivos</h3>
			<ul>
				<li><a href='ccode/soundtrack.ogg' download>Som de fundo</a></li>
				<li><a href='ccode/aiao.ogg' download>Som retângulo azul</a></li>
				<li><a href='ccode/wololo.ogg' download>Som retângulo vermelho</a></li>
			</ul>

			<h3>Outros links</h3>
			<ul>
				<li><a target='_blank' href='http://www.rafaeltoledo.net/tutorial-allegro-5-7-manipulando-audio/'>Manipulando Áudio</a></li>
				<li><a target='_blank' href='https://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial/Addons/Audio'>Allegro 5 Tutorial/Addons/Audio</a></li>
			</ul>
		</div>
		<div id='btn-footer'>
			<a class="button-red" href='teclado.php'>Anterior</a>
			<a class="button-red" href='timer.php'>Próximo</a>
		</div>
	</div>
</div>
</body>
<script src="script/line_link.js"></script>
