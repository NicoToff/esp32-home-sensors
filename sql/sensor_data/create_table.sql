CREATE TABLE sensor_data (
    timestamp INTEGER DEFAULT (strftime('%s', 'now')),
    location INTEGER,
    type INTEGER,
    value INTEGER, -- For a temperature, this should be in Celsius, rounded to nearest .5 and multiplied by 10
    PRIMARY KEY (timestamp, location, type)
);

CREATE TABLE sensor_checkins (
    sensor_id INTEGER PRIMARY KEY,
    last_timestamp INTEGER DEFAULT (strftime('%s', 'now'))
);