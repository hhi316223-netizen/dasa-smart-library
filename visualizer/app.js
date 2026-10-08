let systemState = null;

// Hàm tiện ích: Đổi Unix Timestamp sang chuỗi DD/MM/YYYY
function formatTimestamp(ts) {
    if (!ts || ts === 0) return "Chưa mượn (Available)";
    const date = new Date(ts * 1000);
    const day = String(date.getDate()).padStart(2, '0');
    const month = String(date.getMonth() + 1).padStart(2, '0');
    const year = date.getFullYear();
    return `${day}/${month}/${year}`;
}

// Hàm tiện ích: Đổi chuỗi YYYY-MM-DD từ input date sang Unix Timestamp (giây)
function dateStringToTimestamp(dateStr, isEndOfDay = false) {
    if (!dateStr) return 0;
    const parts = dateStr.split('-');
    const year = parseInt(parts[0], 10);
    const month = parseInt(parts[1], 10) - 1;
    const day = parseInt(parts[2], 10);
    const date = isEndOfDay ? new Date(year, month, day, 23, 59, 59) : new Date(year, month, day, 0, 0, 0);
    return Math.floor(date.getTime() / 1000);
}

// Thuật toán Polynomial Rolling Hash (p = 31) mô phỏng đúng C++
function calculateHashIndex(key, capacity) {
    if (!capacity || capacity === 0) return 0;
    let hashVal = 0;
    const p = 31;
    for (let i = 0; i < key.length; i++) {
        hashVal = (hashVal * p + key.charCodeAt(i)) % capacity;
    }
    return hashVal;
}

// Bắt sự kiện nạp file state.json
document.getElementById('jsonFileInput').addEventListener('change', function(e) {
    const file = e.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(event) {
        try {
            systemState = JSON.parse(event.target.result);
            renderSystemState(systemState);
        } catch (err) {
            alert("Lỗi: Tệp state.json không hợp lệ!");
        }
    };
    reader.readAsText(file);
});

// Render trạng thái tổng thể
function renderSystemState(data) {
    document.getElementById('statTotal').innerText = (data.total || 0).toLocaleString();
    document.getElementById('statCapacity').innerText = (data.hash_capacity || 0).toLocaleString();
    document.getElementById('statLoadFactor').innerText = (data.load_factor || 0).toFixed(4);

    let buckets = data.hash_buckets || [];
    
    // Nếu mảng hash_buckets rỗng, tự động dựng lại bucket từ items theo hàm băm p=31
    if (buckets.length === 0 && data.items && data.items.length > 0) {
        const bucketMap = {};
        data.items.forEach(item => {
            const idx = calculateHashIndex(item.id, data.hash_capacity || 1009);
            if (!bucketMap[idx]) bucketMap[idx] = [];
            bucketMap[idx].push(item);
        });
        buckets = Object.keys(bucketMap).map(k => ({
            bucket: parseInt(k, 10),
            chain: bucketMap[k]
        }));
    }

    document.getElementById('statActiveBuckets').innerText = buckets.length.toLocaleString();
    renderBuckets(buckets);
    renderItemsTable(data.items || []);
}

// Render các hàng Bucket và chuỗi Chaining
function renderBuckets(buckets) {
    const container = document.getElementById('bucketsList');
    container.innerHTML = "";

    if (buckets.length === 0) {
        container.innerHTML = `<p style="color: var(--text-sub);">Bảng băm hiện đang rỗng.</p>`;
        return;
    }

    buckets.forEach(b => {
        const row = document.createElement('div');
        row.className = 'bucket-row';
        row.id = `bucket-row-${b.bucket}`;

        let chainHtml = "";
        (b.chain || []).forEach(node => {
            chainHtml += `
                <div class="node-box" id="node-${node.id}">
                    <strong>${node.id}</strong>: ${node.title} 
                    <span style="color: var(--accent-orange); font-size: 0.75rem;">(P:${node.priority})</span>
                </div>
                <span class="arrow">&rarr;</span>
            `;
        });
        chainHtml += `<span style="color: var(--text-sub); font-size: 0.8rem;">nullptr</span>`;

        row.innerHTML = `
            <div class="bucket-label">Bucket [${b.bucket}]</div>
            <div class="chain-nodes">${chainHtml}</div>
        `;
        container.appendChild(row);
    });
}

// Render bảng toàn bộ tài liệu
function renderItemsTable(items) {
    const tbody = document.getElementById('rawItemsTableBody');
    tbody.innerHTML = "";
    items.forEach(it => {
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td><strong>${it.id}</strong></td>
            <td>${it.title}</td>
            <td><span style="color: var(--accent-orange);">Mức ${it.priority}</span></td>
            <td>${formatTimestamp(it.dueDate)}</td>
        `;
        tbody.appendChild(tr);
    });
}

// ==========================================
// XỬ LÝ 3 CHỨC NĂNG CỐT LÕI
// ==========================================

// 1. Chức năng 1 (MC1): Tra cứu theo ID
function handleSearchMC1() {
    if (!systemState || !systemState.items) {
        alert("Vui lòng nạp file state.json trước!");
        return;
    }
    const searchId = document.getElementById('inputSearchId').value.trim();
    if (!searchId) return;

    const found = systemState.items.find(item => item.id.toUpperCase() === searchId.toUpperCase());
    const resBox = document.getElementById('resultMC1');

    if (found) {
        resBox.innerHTML = `
            <span style="color: var(--accent-green); font-weight: bold;">[TÌM THẤY]</span><br>
            <strong>Mã sách:</strong> ${found.id} | <strong>Tựa đề:</strong> ${found.title}<br>
            <strong>Mức ưu tiên:</strong> ${found.priority} | <strong>Hạn trả:</strong> ${formatTimestamp(found.dueDate)}
        `;
        const expectedBucket = calculateHashIndex(found.id, systemState.hash_capacity || 1009);
        const rowEl = document.getElementById(`bucket-row-${expectedBucket}`);
        if (rowEl) {
            rowEl.scrollIntoView({ behavior: 'smooth', block: 'center' });
            rowEl.style.borderColor = 'var(--primary)';
            setTimeout(() => { rowEl.style.borderColor = 'var(--border-color)'; }, 3000);
        }
    } else {
        resBox.innerHTML = `<span style="color: #f87171; font-weight: bold;">[KHÔNG TÌM THẤY]</span> Mã ${searchId} không tồn tại trong hệ thống.`;
    }
}

// 2. Chức năng 2 (FR1): Rút tài liệu ưu tiên cao nhất (Min-Heap)
function handlePollFR1() {
    if (!systemState || !systemState.items || systemState.items.length === 0) {
        alert("Hệ thống chưa có dữ liệu!");
        return;
    }
    // Tìm phần tử có priority nhỏ nhất (1 là cao nhất)
    let best = systemState.items[0];
    for (let i = 1; i < systemState.items.length; i++) {
        if (systemState.items[i].priority < best.priority) {
            best = systemState.items[i];
        }
    }

    const resBox = document.getElementById('resultFR1');
    resBox.innerHTML = `
        <span style="color: var(--accent-purple); font-weight: bold;">[ĐIỀU PHỐI THÀNH CÔNG]</span><br>
        <strong>Mã:</strong> ${best.id} &mdash; <strong>${best.title}</strong><br>
        <strong>Mức ưu tiên cấp phát:</strong> <span style="color: var(--accent-orange); font-weight: bold;">Cấp độ ${best.priority}</span>
    `;
}

// 3. Chức năng 3 (FR2): Kiểm toán theo khoảng thời gian (LỌC KHOẢNG NGÀY)
function handleAuditRangeFR2() {
    if (!systemState || !systemState.items) {
        alert("Vui lòng nạp file state.json trước!");
        return;
    }

    const startStr = document.getElementById('inputStartDate').value;
    const endStr = document.getElementById('inputEndDate').value;

    if (!startStr || !endStr) {
        alert("Vui lòng chọn cả ngày bắt đầu và ngày kết thúc!");
        return;
    }

    const startTs = dateStringToTimestamp(startStr, false);
    const endTs = dateStringToTimestamp(endStr, true);

    if (startTs > endTs) {
        alert("Lỗi: Ngày bắt đầu không được lớn hơn ngày kết thúc!");
        return;
    }

    // Lọc các bản ghi có dueDate nằm trong khoảng [startTs, endTs]
    const filtered = systemState.items.filter(item => {
        return item.dueDate && item.dueDate >= startTs && item.dueDate <= endTs;
    });

    // Sắp xếp tăng dần theo dueDate (mô phỏng đúng trật tự In-order của Cây AVL)
    filtered.sort((a, b) => a.dueDate - b.dueDate);

    // Cập nhật giao diện
    const resBox = document.getElementById('resultFR2');
    const auditSection = document.getElementById('auditResultSection');
    const tbody = document.getElementById('auditTableBody');
    const badge = document.getElementById('auditCountBadge');

    resBox.innerHTML = `
        <span style="color: var(--accent-green); font-weight: bold;">[KIỂM TOÁN HOÀN TẤT]</span><br>
        Tìm thấy <strong>${filtered.length}</strong> bản ghi trong khoảng từ <strong>${formatTimestamp(startTs)}</strong> đến <strong>${formatTimestamp(endTs)}</strong>.
    `;

    tbody.innerHTML = "";
    badge.innerText = `${filtered.length} bản ghi`;

    if (filtered.length > 0) {
        filtered.forEach(it => {
            const tr = document.createElement('tr');
            tr.innerHTML = `
                <td><strong>${it.id}</strong></td>
                <td>${it.title}</td>
                <td><span style="color: var(--accent-orange);">Mức ${it.priority}</span></td>
                <td><strong style="color: var(--primary);">${formatTimestamp(it.dueDate)}</strong></td>
                <td style="color: var(--text-sub);">${it.dueDate}</td>
            `;
            tbody.appendChild(tr);
        });
        auditSection.style.display = 'block';
        auditSection.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
    } else {
        tbody.innerHTML = `<tr><td colspan="5" style="text-align: center; color: var(--text-sub);">Không có tài liệu nào đến hạn trong khoảng ngày này.</td></tr>`;
        auditSection.style.display = 'block';
    }
}