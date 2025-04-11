export default () => {
	// Helper function to hide an element
	function hideElement(element) {
		if (element) element.style.display = 'none';
	}
	
	// Helper function to show an element
	function showElement(element) {
		if (element) element.style.display = 'block';
	}
	
	// Helper function to get elements that don't match selectors
	function getElementsNotMatching(selector, notSelectors) {
		const elements = document.querySelectorAll(selector);
		const notElements = document.querySelectorAll(notSelectors);
		const notElementsArray = Array.from(notElements);
		
		return Array.from(elements).filter(el => !notElementsArray.includes(el));
	}
	
	// Get elements
	const disableMenu = document.getElementById('disable-menu');
	const fog = document.querySelector('.fog');
	const headerToggle = document.querySelector('.header-toggle');
	const sideMenu = document.querySelector('.side-menu');
	
	// Disable menus logic
	if (disableMenu && disableMenu.innerHTML === 'disciplinas') {
		const menuItems = getElementsNotMatching('.side-menu-list li', '#profile, #sair, #disciplinas');
		menuItems.forEach(item => {
			item.style.pointerEvents = 'none';
			item.style.opacity = '0.3';
		});
	} else if (disableMenu && disableMenu.innerHTML === 'questoes') {
		const menuItems = getElementsNotMatching('.side-menu-list li', '#profile, #sair, #disciplinas, #inserir');
		menuItems.forEach(item => {
			item.style.pointerEvents = 'none';
			item.style.opacity = '0.3';
		});
	}
	
	// Fog click event
	if (fog) {
		fog.addEventListener('click', function() {
			hideElement(fog);
			if (window.innerWidth < 1200) {
				showElement(headerToggle);
				hideElement(sideMenu);
			}
		});
	}
	
	// Header toggle click event
	if (headerToggle) {
		headerToggle.addEventListener('click', function() {
			this.style.opacity = '1';
			this.style.backgroundColor = '#888';
			this.style.backgroundImage = 'url(#)';
			
			// Simple animation to mimic jQuery's animate
			this.style.transition = 'width 200ms, height 200ms';
			this.style.width = '260px';
			this.style.height = '100%';
			
			// Use setTimeout to simulate the callback
			setTimeout(() => {
				this.removeAttribute('style');
				hideElement(this);
				showElement(sideMenu);
				
				// Show fog with fade effect
				if (fog) {
					fog.style.display = 'block';
					fog.style.opacity = '0';
					fog.style.transition = 'opacity 100ms';
					setTimeout(() => {
						fog.style.opacity = '1';
					}, 10);
				}
			}, 200);
		});
	}
	
	// Window resize event
	window.addEventListener('resize', function() {
		if (window.innerWidth >= 1200) {
			if (sideMenu) showElement(sideMenu);
			if (fog) fog.click(); // Trigger the fog click event
		} else {
			if (sideMenu) hideElement(sideMenu);
		}
	});
}

