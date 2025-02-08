CREATE TABLE sensor_data (
    timestamp INTEGER DEFAULT (strftime('%s', 'now')),
    location INTEGER,
    type INTEGER,
    value INTEGER, -- For a temperature, this should be in Celsius, rounded to nearest .5 and multiplied by 10
    PRIMARY KEY (timestamp, location, type)
);

CREATE TABLE sensor_checkins (
    sensor_id INTEGER PRIMARY KEY,
    last_timestamp INTEGER DEFAULT (unixepoch())
);

CREATE TABLE cache (
    key      INTEGER PRIMARY KEY,  -- Unique INT identifier for the cached item
    value    TEXT,                 -- Cached data (JSON, string, etc.)
    expiry   INTEGER DEFAULT (unixepoch() + 900) -- Default expiry is 15 mins
);
CREATE INDEX idx_expiry_value ON cache(expiry, value);
CREATE INDEX idx_expiry ON cache(expiry);