
LOAD	0
PUSH
STACKW	0
READ	V0

LOAD	V0

PUSH	
STACKW	0
IN:	NOOP

LOAD	0
STORE	V1
STACKR	0
SUB		V1
BRZNEG	OUT
STACKR	0
STORE	V2

WRITE	V2

LOAD	2
STORE	V3

STACKR	0
SUB	V3

STORE	V4

STACKW	0
BR	IN
OUT:	NOOP

STACKR	0
STORE	V5

WRITE	V5

LOAD	2
STORE	V6

STACKR	0
SUB	V6

STORE	V7

STACKW	0
STOP
V0	0
V1	0
V2	0
V3	0
V4	0
V5	0
V6	0
V7	0
