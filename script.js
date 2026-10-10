(function () {
  // Mobile menu
  var nav = document.getElementById('site-nav');
  var btn = document.querySelector('.menu-btn');
  if (nav && btn) {
    btn.addEventListener('click', function () {
      var open = nav.classList.toggle('open');
      btn.setAttribute('aria-expanded', open ? 'true' : 'false');
      btn.textContent = open ? 'Close' : 'Menu';
    });
  }

  // Tabs (ProtoSem page): buttons[role=tab][data-tab] <-> [role=tabpanel]#id
  var tabs = document.querySelectorAll('.tab');
  function showTab(id, push) {
    var found = false;
    tabs.forEach(function (t) { if (t.getAttribute('data-tab') === id) found = true; });
    if (!found) id = tabs[0].getAttribute('data-tab');
    tabs.forEach(function (t) { t.setAttribute('aria-selected', t.getAttribute('data-tab') === id ? 'true' : 'false'); });
    document.querySelectorAll('.tab-panel').forEach(function (p) { p.hidden = p.id !== id; });
    if (push) { try { history.replaceState(null, '', '#' + id); } catch (e) {} }
  }
  if (tabs.length) {
    tabs.forEach(function (t) { t.addEventListener('click', function () { showTab(t.getAttribute('data-tab'), true); }); });
    showTab((location.hash || '').replace('#', ''), false);
    window.addEventListener('hashchange', function () { showTab((location.hash || '').replace('#', ''), false); });
  }

  // Copy buttons
  document.querySelectorAll('.copy').forEach(function (b) {
    b.addEventListener('click', function () {
      var el = document.querySelector(b.getAttribute('data-copy'));
      if (!el) return;
      var label = b.textContent;
      var reset = function (t) { b.textContent = t; setTimeout(function () { b.textContent = label; }, 1600); };
      var pick = function () {
        var r = document.createRange(); r.selectNodeContents(el);
        var s = window.getSelection(); s.removeAllRanges(); s.addRange(r); reset('Selected');
      };
      try { navigator.clipboard.writeText(el.textContent.trim()).then(function () { reset('Copied'); }, pick); }
      catch (e) { pick(); }
    });
  });

  // Project filters
  var fb = document.querySelectorAll('.filters button');
  fb.forEach(function (f) {
    f.addEventListener('click', function () {
      var want = f.getAttribute('data-filter');
      fb.forEach(function (o) { o.setAttribute('aria-pressed', o === f ? 'true' : 'false'); });
      document.querySelectorAll('[data-kind]').forEach(function (p) {
        p.hidden = !(want === 'all' || p.getAttribute('data-kind') === want);
      });
    });
  });

  document.querySelectorAll('[data-year]').forEach(function (y) { y.textContent = new Date().getFullYear(); });
})();

// Photo slots: show a labelled placeholder until the image file exists
(function () {
  function holder(img) {
    if (img.dataset.ph) return;
    img.dataset.ph = '1';
    var d = document.createElement('div');
    d.className = 'ph';
    d.innerHTML = '<b>Add photo</b><span>Save as</span><code></code>';
    d.querySelector('code').textContent = 'images/iot/task1/' + (img.getAttribute('data-file') || '');
    img.replaceWith(d);
  }
  document.querySelectorAll('img.photo').forEach(function (img) {
    img.addEventListener('error', function () { holder(img); });
    if (img.complete && img.naturalWidth === 0) holder(img);
  });
})();
