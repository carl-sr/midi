# Midi Structure c++
Load and save midi files into c++ data structures.


## MThd
```c++
class MThd {
	private:
		u_int32_t chunk_type;
		u_int32_t length;
		u_int16_t format;
		u_int16_t ntrks;
		u_int16_t division;
};
```
Contains all midi track header information. Each midi file contains one MThd and any number of MTrk.

## MTrk
```c++
class MTrk {
	private:
		u_int32_t chunk_type;
		u_int32_t length;
		std::vector<std::shared_ptr<MTrk_Event>> track_events;
};
```
Contains a vector of MTrk events as well as some information about the MTrk chunk.

## MTrk_Event
```c++
class MTrk_Event {
	protected:
		int delta_time {0};
};
```
Base class for different events. delta_time is the start time of the event.

## Midi_Event
```c++
class Midi_Event : public MTrk_Event {
	private:
		u_int8_t function;
		u_int8_t fb;
		u_int8_t sb;
};
```
Extended from MTrk_Event. function, fb, and sb value depend on midi event type. Some events do not use the full three bytes.

## Meta_Event
```c++
class Meta_Event : public MTrk_Event {
	private:
		u_int8_t type;
		std::vector<u_int8_t> data;
};
```
Extended from MTrk_Event. Contains a arbitrary amount of data stored as a variable length bit pattern.

## Sys_Ex_Event
```c++
class Sys_Ex_Event : public MTrk_Event {
	private:
		std::vector<u_int8_t> data;
};
```
Extended from MTrk_Event. Similiar to Meta_Event.
