let appState = null;

// Chuyển đổi Tab
function switchTab(tabId) {
    document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
    document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
    document.querySelector(`[onclick="switchTab('${tabId}')"]`).classList.add('active');
    document.getElementById(tabId).classList.add('active');
}

// Đọc file JSON an toàn qua FileReader (Không bị lỗi CORS)
document.getElementById('jsonFileInput').addEventListener('change', function(e) {
    const file = e.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(event) {
        try {
            appState = JSON.parse(event.target.result);
            renderDashboard();
        } catch (err) {
            alert('File JSON không đúng định dạng!');
        }
    };
    reader.readAsText(file);
});

// Cập nhật toàn bộ giao diện
function renderDashboard() {
    if (!appState) return;

    // 1. Cập nhật chỉ số thống kê
    document.getElementById('statTotal').innerText = appState.total || 0;
    document.getElementById('statCapacity').innerText = appState.hash_capacity || 0;
    document.getElementById('statLoadFactor').innerText = (appState.load_factor || 0).toFixed(3);
    document.getElementById('statBucketsUsed').innerText = appState.hash_buckets ? appState.hash_buckets.length : 0;

    renderHashTable();
    renderPriorityWaitlist();
    renderRawTable();
}

// 2. Render trực quan hóa Bảng băm & Chaining
function renderHashTable(targetBucket = -1, targetKey = "") {
    const container = document.getElementById('bucketContainer');
    container.innerHTML = '';

    if (!appState.hash_buckets || appState.hash_buckets.length === 0) {
        container.innerHTML = '<div style="text-align: center; color: #94a3b8; padding: 40px;">Bảng băm hiện tại đang rỗng.</div>';
        return;
    }

    appState.hash_buckets.forEach(b => {
        const isTarget = b.bucket === targetBucket;
        const row = document.createElement('div');
        row.className = `bucket-row ${isTarget ? 'highlight' : ''}`;
        row.id = `bucket-row-${b.bucket}`;

        let nodesHtml = '';
        b.chain.forEach(node => {
            const isMatch = node.id === targetKey;
            nodesHtml += `
                <div class="node-card ${isMatch ? 'active-match' : ''}">
                    <span class="id">${node.id}</span>
                    <span class="title" title="${node.title}">${node.title}</span>
                    <span class="badge p-${node.priority}">Mức ${node.priority}</span>
                </div>
                <span class="arrow">→</span>
            `;
        });
        nodesHtml += `<span class="null-node">NULL</span>`;

        row.innerHTML = `
            <div class="bucket-idx">Bucket #${b.bucket}</div>
            <div class="chain-nodes">${nodesHtml}</div>
        `;
        container.appendChild(row);
    });

    if (targetBucket !== -1) {
        const targetEl = document.getElementById(`bucket-row-${targetBucket}`);
        if (targetEl) targetEl.scrollIntoView({ behavior: 'smooth', block: 'center' });
    }
}

// 3. Mô phỏng hàm băm Polynomial Hash khớp 100% với C++
function simulateHashSearch() {
    const key = document.getElementById('hashSearchInput').value.trim();
    if (!key || !appState) return;

    const capacity = appState.hash_capacity;
    let hashVal = 0;
    const p = 31;
    for (let i = 0; i < key.length; i++) {
        hashVal = (hashVal * p + key.charCodeAt(i)) % capacity;
    }

    document.getElementById('hashFormulaText').innerHTML = 
        `Mã <b>${key}</b> băm ra: index = <b>${hashVal}</b> (trong tổng số ${capacity} buckets)`;

    renderHashTable(hashVal, key);
}

// 4. Render Hàng đợi ưu tiên (Sắp xếp theo Priority Level 1 -> 5)
function renderPriorityWaitlist() {
    const tbody = document.getElementById('priorityTableBody');
    tbody.innerHTML = '';
    if (!appState.items) return;

    // Sắp xếp bản sao theo thứ tự ưu tiên tăng dần (1 là cao nhất)
    const sorted = [...appState.items].sort((a, b) => a.priority - b.priority);

    sorted.slice(0, 30).forEach(item => {
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td><b>${item.id}</b></td>
            <td>${item.title}</td>
            <td><span class="badge p-${item.priority}">Cấp ${item.priority}</span></td>
            <td>${item.dueDate}</td>
        `;
        tbody.appendChild(tr);
    });
}

// 5. Render Bảng dữ liệu thô
function renderRawTable() {
    const tbody = document.getElementById('rawTableBody');
    tbody.innerHTML = '';
    if (!appState.items) return;

    appState.items.slice(0, 50).forEach((item, idx) => {
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td>${idx + 1}</td>
            <td><b>${item.id}</b></td>
            <td>${item.title}</td>
            <td><span class="badge p-${item.priority}">Mức ${item.priority}</span></td>
            <td>${item.dueDate}</td>
        `;
        tbody.appendChild(tr);
    });
}