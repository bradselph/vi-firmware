#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

#define ByteQueue QUEUE_TYPE(uint8_t)

#define QUEUE_MAX_LENGTH(type) queue_##type##_max_length
#define QUEUE_MAX_INTERNAL_LENGTH(type) queue_##type##_max_internal_length

#define QUEUE_DECLARE(type, max_length) \
const int queue_##type##_max_length = max_length; \
const int queue_##type##_max_internal_length = max_length + 1; \
typedef struct queue_##type##_s { \
    int head; \
    int tail; \
    type elements[QUEUE_MAX_INTERNAL_LENGTH(type)]; \
} queue_##type; \
\
bool queue_##type##_push(queue_##type* queue, type value); \
\
type queue_##type##_pop(queue_##type* queue); \
\
void queue_init(queue_##type* queue); \
int queue_length(queue_##type* queue); \
bool queue_full(queue_##type* queue); \
bool queue_empty(queue_##type* queue); \
void queue_snapshot(queue_##type* queue, type* snapshot);

#define QUEUE_DEFINE(type) \
bool queue_##type##_push(queue_##type* queue, type value) { \
    int next = (queue->head + 1) % (QUEUE_MAX_INTERNAL_LENGTH(type)); \
    if (next == queue->tail) { \
        return false; \
    } \
    \
    queue->elements[queue->head] = value; \
    queue->head = next; \
    \
    return true; \
} \
\
type queue_##type##_pop(queue_##type* queue){ \
	if (queue->head == queue->tail) { \
		return 0; \
	} \
\
	int next = (queue->tail + 1) % QUEUE_MAX_INTERNAL_LENGTH(type); \
	type value = queue->elements[queue->tail]; \
	queue->tail = next; \
\
    return value; \
} \
\
void queue_init(queue_##type* queue) { \
    queue->head = queue->tail = 0; \
} \
\
int queue_length(queue_##type* queue) { \
	return (QUEUE_MAX_INTERNAL_LENGTH(type) + queue->head - queue->tail) \
            % QUEUE_MAX_INTERNAL_LENGTH(type); \
} \
\
bool queue_full(queue_##type* queue) { \
    return queue_length(queue) == QUEUE_MAX_LENGTH(type); \
} \
\
bool queue_empty(queue_##type* queue) { \
    return queue_length(queue) == 0; \
} \
\
void queue_snapshot(queue_##type* queue, type* snapshot) { \
    int i; \
    for(i = 0; i < queue_length(queue); i++) { \
        snapshot[i] = queue->elements[ \
            (queue->tail + i) % QUEUE_MAX_INTERNAL_LENGTH(type)]; \
    } \
}

#define QUEUE_TYPE(type) queue_##type

#define QUEUE_PUSH(type, queue, value) queue_##type##_push(queue, value)

#define QUEUE_POP(type, queue) queue_##type##_pop(queue)

QUEUE_DECLARE(uint8_t, 512)

#endif // _QUEUE_H_
