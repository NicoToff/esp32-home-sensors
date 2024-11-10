CREATE TABLE sensor_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp INTEGER DEFAULT (strftime('%s', 'now')),
    location INTEGER,
    type INTEGER,
    value REAL
);