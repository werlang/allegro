// This JavaScript file replaces the PHP functions.php functionality
// for loading and displaying code examples

import line_link from "./line_link.js";

async function loadCodeFile(filePath, elementId, lineNum = null) {
    return fetch(filePath)
        .then(response => response.text())
        .then(content => {
            // If lineNum is specified, extract just that line
            if (lineNum !== null && lineNum > 0) {
                const lines = content.split('\n');
                if (lineNum <= lines.length) {
                    content = lines[lineNum - 1];
                }
            }
            
            // Escape HTML entities to show code as text
            // content = escapeHtml(content);
            
            // Insert the code into the specified element
            const element = document.getElementById(elementId);
            if (element.classList.contains('prettyprinted')) {
                element.classList.remove('prettyprinted');
            }
            element.textContent = content;
            
            // Apply syntax highlighting if using prettify
            if (typeof PR !== 'undefined') {
                PR.prettyPrint();
            }
        })
        .catch(error => console.error('Error loading code file:', error));
}

// Helper function to escape HTML entities
function escapeHtml(unsafe) {
    return unsafe
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}

// Function to load all code examples on page load
document.addEventListener('DOMContentLoaded', function() {
    // Find all elements with data-code-file attribute
    const codeElements = document.querySelectorAll('[data-code-file]');
    Promise.all(Array.from(codeElements).map(async element => {
        const filePath = element.getAttribute('data-code-file');
        const lineNum = element.getAttribute('data-line-num');
        
        // Load code into this element
        return await loadCodeFile(filePath, element.id, lineNum ? parseInt(lineNum) : null);
    })).then(() => {
        line_link();
        PR.prettyPrint();
    })

});
