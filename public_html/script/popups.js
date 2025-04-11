function summon_tooltip(obj, pos, text) {
	// Fade out existing tooltips
	const existingTooltips = document.querySelectorAll('.tooltip');
	existingTooltips.forEach(tooltip => {
		// Simple fade out effect
		tooltip.style.transition = 'opacity 500ms';
		tooltip.style.opacity = '0';
		setTimeout(() => {
			if (tooltip.parentNode) tooltip.style.display = 'none';
		}, 500);
	});

	// Remove appended tooltips
	const appendedTooltips = document.querySelectorAll('.tooltip.append');
	appendedTooltips.forEach(tooltip => {
		if (tooltip.parentNode) tooltip.parentNode.removeChild(tooltip);
	});

	if (!obj) return;
	var objname = obj.getAttribute("name");

	if (!text) {
		const tooltipSelector = `.tooltip[data-tooltipfor='${objname}']`;
		const tooltipobj = document.querySelector(tooltipSelector);
		if (tooltipobj) {
			// Get element dimensions and position
			const objRect = obj.getBoundingClientRect();
			const tooltipRect = tooltipobj.getBoundingClientRect();

			let left, top;

			if (tooltipobj.classList.contains('up')) {
				left = objRect.left + objRect.width / 2 - tooltipRect.width / 2;
				top = objRect.top - tooltipRect.height - 5;
			}
			if (tooltipobj.classList.contains('down')) {
				left = objRect.left + objRect.width / 2 - tooltipRect.width / 2;
				top = objRect.top + objRect.height + 5;
			}
			if (tooltipobj.classList.contains('left')) {
				top = objRect.top + objRect.height / 2 - tooltipRect.height / 2;
				left = objRect.left - tooltipRect.width - 5;
			}
			if (tooltipobj.classList.contains('right')) {
				top = objRect.top + objRect.height / 2 - tooltipRect.height / 2;
				left = objRect.left + objRect.width + 5;
			}

			// Adjust position to keep tooltip within the viewport
			if (left < 0) left = objRect.left;
			if (left + tooltipRect.width > window.innerWidth) {
				left = objRect.right - tooltipRect.width;
			}
			if (top < 0) top = objRect.bottom + 5;
			if (top + tooltipRect.height > window.innerHeight) {
				top = objRect.top - tooltipRect.height - 5;
			}

			tooltipobj.style.position = 'fixed';
			tooltipobj.style.left = left + 'px';
			tooltipobj.style.top = top + 'px';
			tooltipobj.style.display = 'block';
		}
	} else {
		text = text.replace(/>/g, '&gt;').replace(/</g, '&lt;');
		// Create tooltip element and append it
		const tooltipElement = document.createElement('div');
		tooltipElement.className = 'tooltip append';
		tooltipElement.innerHTML = text;
		obj.parentNode.append(tooltipElement);

		var tooltipobj = document.querySelector(".tooltip.append");
		const objRect = obj.getBoundingClientRect();
		const tooltipRect = tooltipobj.getBoundingClientRect();

		let left;

		if (pos == 'up')
			left = objRect.left + objRect.width / 2 - tooltipRect.width / 2;
		if (pos == 'down')
			left = objRect.left + objRect.width / 2 - tooltipRect.width / 2;
		if (pos == 'left')
			left = objRect.left - tooltipRect.width - 5;
		if (pos == 'right')
			left = objRect.left + objRect.width + 5;

		// Adjust position to keep tooltip within the viewport
		if (left < 0) left = objRect.left;
		var right = left + tooltipRect.width;
		if (right >= window.innerWidth) {
			left = objRect.right - tooltipRect.width;
			if (left < 0) {
				tooltipobj.style.width = (tooltipRect.width + left) + 'px';
				left = objRect.left;
			}
		}

		let top;
		if (pos == 'up')
			top = objRect.top - tooltipRect.height - 5;
		if (pos == 'down')
			top = objRect.top + objRect.height + 5;
		if (pos == 'left')
			top = objRect.top + objRect.height / 2 - tooltipRect.height / 2;
		if (pos == 'right')
			top = objRect.top + objRect.height / 2 - tooltipRect.height / 2;

		// Adjust position to keep tooltip within the viewport
		if (top < 0) top = objRect.bottom + 5;
		if (top + tooltipRect.height > window.innerHeight) {
			top = objRect.top - tooltipRect.height - 5;
		}

		tooltipobj.style.position = 'fixed';
		tooltipobj.style.left = left + 'px';
		tooltipobj.style.top = top + 'px';
		tooltipobj.style.display = 'block';
	}
}

function create_popup(content, divclass, dimension, url) {
    // Create popup elements
    const container = document.querySelector('.container');
    const popupElement = document.createElement('div');
    popupElement.className = 'popup';

    const innerDiv = document.createElement('div');
    innerDiv.className = divclass;
    
    // Append elements
    popupElement.appendChild(innerDiv);
    container.appendChild(popupElement);
    
    // Add click event to close popup
    popupElement.addEventListener('click', function(e) {
        if (e.target === this || divclass === "message") {
            if (popupElement.parentNode) {
                popupElement.parentNode.removeChild(popupElement);
            }
        }
    });
    
    // Handle window popup styling
    if (divclass === 'window') {
        innerDiv.style.width = dimension + 'px';
        
        if (window.innerWidth < dimension) {
            innerDiv.classList.add('m-size');
        } else {
            innerDiv.classList.add('f-size');
        }
    }
    
    // Set content
    innerDiv.innerHTML = content;
    popupElement.style.display = 'block';
    
    // Add URL redirect if provided
    if (url) {
        popupElement.addEventListener('click', function() {
            window.location.href = url;
        });
    }
}

function show_message(text, url) {
    create_popup(text, "message", null, url);
}
