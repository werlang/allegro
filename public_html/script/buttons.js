document.addEventListener('DOMContentLoaded', function() {
    document.addEventListener('click', function(event) {
        if (event.target.classList.contains('button-red') && event.target.classList.contains('can-disable')) {
            event.target.setAttribute('disabled', true);
            const loadingImg = document.createElement('img');
            loadingImg.src = 'icons/loading.gif';
            loadingImg.className = 'loadimg';
            event.target.insertAdjacentElement('afterbegin', loadingImg);
        }
    });
});

function enable_button(obj) {
    obj.disabled = false;
    const loadImg = obj.querySelector('.loadimg');
    if (loadImg) {
        loadImg.remove();
    }
}