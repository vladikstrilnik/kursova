import sqlite3

DB_NAME = "water.db"

def get_connection():
    return sqlite3.connect(DB_NAME)

def init_db():
    conn = get_connection()

    conn.execute("""
        CREATE TABLE IF NOT EXISTS water_log(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            device TEXT,
            liters REAL,
            flow REAL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    """)

    conn.commit()
    conn.close()
