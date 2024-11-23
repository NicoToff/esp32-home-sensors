CREATE TABLE sensor_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp INTEGER DEFAULT (strftime('%s', 'now')),
    location INTEGER,
    type INTEGER,
    value INTEGER -- For a temperature, this should be in Celsius, rounded to nearest .5 and multiplied by 10
);