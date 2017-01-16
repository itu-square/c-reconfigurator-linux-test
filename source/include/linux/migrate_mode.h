// /*AFLA*/ #ifndef MIGRATE_MODE_H_INCLUDED
// /*AFLA*/ #define MIGRATE_MODE_H_INCLUDED
// /*AFLA*/ /*
// /*AFLA*/  * MIGRATE_ASYNC means never block
// /*AFLA*/  * MIGRATE_SYNC_LIGHT in the current implementation means to allow blocking
// /*AFLA*/  *	on most operations but not ->writepage as the potential stall time
// /*AFLA*/  *	is too significant
// /*AFLA*/  * MIGRATE_SYNC will block when migrating pages
// /*AFLA*/  */
// /*AFLA*/ enum migrate_mode {
// /*AFLA*/ 	MIGRATE_ASYNC,
// /*AFLA*/ 	MIGRATE_SYNC_LIGHT,
// /*AFLA*/ 	MIGRATE_SYNC,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif		/* MIGRATE_MODE_H_INCLUDED */
