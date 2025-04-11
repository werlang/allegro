// Side menu loader script
// This replaces the PHP include functionality by dynamically loading the side menu content

import side_menu from "./side_menu.js";

document.addEventListener('DOMContentLoaded', function() {
    // Find the element with id 'side-menu-container'
    const sideMenuContainer = document.getElementById('side-menu-container');
    
    if (sideMenuContainer) {
        // Fetch the side menu HTML
        fetch('side_menu.html')
            .then(response => response.text())
            .then(html => {
                // Insert the side menu HTML
                sideMenuContainer.innerHTML = html;
                
                // Update active menu item based on current page
                highlightCurrentPage();

                side_menu();
            })
            .catch(error => console.error('Error loading side menu:', error));
    }
});

// Function to highlight the current page in the menu
function highlightCurrentPage() {
    // Get the current page filename
    const currentPage = window.location.pathname.split('/').pop() || 'index.html';
    
    // Find the corresponding menu item
    const menuItems = document.querySelectorAll('.side-menu-list a');
    
    menuItems.forEach(item => {
        const href = item.getAttribute('href');
        
        // If this is the current page, add an 'active' class
        if (href === currentPage) {
            item.classList.add('active');
        }
    });
}
