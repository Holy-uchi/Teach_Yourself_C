typedef enum {
  UNINITIALIZED,
  INITIALIZED,
  IN_USE,
  CONFIRMED,
  DELETED,
} STATUS;

STATUS check_status(void);
