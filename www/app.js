// Particle system
function createParticles() {
    const container = document.getElementById('particles');
    const count = 35;

    for (let i = 0; i < count; i++) {
        const p = document.createElement('div');
        p.className = 'particle';
        p.style.left = Math.random() * 100 + '%';
        p.style.animationDuration = (6 + Math.random() * 10) + 's';
        p.style.animationDelay = Math.random() * 8 + 's';
        p.style.width = p.style.height = (1.5 + Math.random() * 2.5) + 'px';
        p.style.opacity = 0.3 + Math.random() * 0.5;

        // Random color between cyan / purple
        const colors = ['#00f0ff', '#7c6aff', '#ff6b9d', '#a78bfa'];
        p.style.background = colors[Math.floor(Math.random() * colors.length)];

        container.appendChild(p);
    }
}

// Test server button
const testBtn = document.getElementById('testBtn');
const responseBox = document.getElementById('responseBox');
const responseContent = document.getElementById('responseContent');
const closeBtn = document.getElementById('closeBtn');

async function hello() {
    testBtn.classList.add('loading');
    testBtn.querySelector('.btn-text').textContent = 'Requesting...';

    responseBox.classList.remove('show');

    try {
        // 真实请求（如果你的服务器有 /hello 接口）
        // const res = await fetch('/hello');
        // const data = await res.text();

        // 模拟延迟 + 漂亮的假响应（方便本地预览）
        await new Promise(r => setTimeout(r, 600 + Math.random() * 400));

        const fakeResponse = {
            status: 200,
            message: "Hello from Chihiro WebServer ⚡",
            server: "Chihiro/1.0",
            protocol: "HTTP/1.1",
            time: new Date().toISOString(),
            features: ["epoll", "non-blocking", "reactor"],
            uptime: Math.floor(Math.random() * 86400) + "s"
        };

        responseContent.textContent = JSON.stringify(fakeResponse, null, 2);
        responseBox.classList.add('show');
    } catch (err) {
        responseContent.textContent = `Error: ${err.message}`;
        responseBox.classList.add('show');
    } finally {
        testBtn.classList.remove('loading');
        testBtn.querySelector('.btn-text').textContent = 'Test Server';
    }
}

testBtn.addEventListener('click', hello);
closeBtn.addEventListener('click', () => {
    responseBox.classList.remove('show');
});

// Feature hover tilt effect (subtle)
document.querySelectorAll('.feature').forEach(el => {
    el.addEventListener('mousemove', (e) => {
        const rect = el.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;
        const centerX = rect.width / 2;
        const centerY = rect.height / 2;
        const rotateX = (y - centerY) / 18;
        const rotateY = (centerX - x) / 18;

        el.style.transform = `perspective(600px) rotateX(${rotateX}deg) rotateY(${rotateY}deg) translateY(-6px)`;
    });

    el.addEventListener('mouseleave', () => {
        el.style.transform = '';
    });
});

// Init
createParticles();

// Keyboard shortcut
document.addEventListener('keydown', (e) => {
    if (e.key === 'Enter' || e.key === ' ') {
        if (document.activeElement === testBtn || e.target === document.body) {
            e.preventDefault();
            hello();
        }
    }
});