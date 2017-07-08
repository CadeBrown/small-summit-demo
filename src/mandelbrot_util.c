
#include "mandelbrot.h"
#include "mandelbrot_util.h"

unsigned int setup_shader(const char *fname) {
	FILE *fp;
	unsigned int prog, sdr, len;
	char *src_buf;
	int success, linked;

	if(!(fp = fopen(fname, "r"))) {
		fprintf(stderr, "failed to open shader: %s\n", fname);
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	src_buf = malloc(len + 1);

	fread(src_buf, 1, len, fp);
	src_buf[len] = 0;

	sdr = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(sdr, 1, (const char**)&src_buf, 0);
	free(src_buf);

	glCompileShaderARB(sdr);
	glGetObjectParameterivARB(sdr, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if(!success) {
		int info_len;
		char *info_log;
		
		glGetObjectParameterivARB(sdr, GL_OBJECT_INFO_LOG_LENGTH_ARB, &info_len);
		if(info_len > 0) {
			if(!(info_log = malloc(info_len + 1))) {
				perror("malloc failed");
				return 0;
			}
			glGetInfoLogARB(sdr, info_len, 0, info_log);
			fprintf(stderr, "shader compilation failed: %s\n", info_log);
			free(info_log);
		} else {
			fprintf(stderr, "shader compilation failed\n");
		}
		return 0;
	}

	prog = glCreateProgramObjectARB();
	glAttachObjectARB(prog, sdr);
	glLinkProgramARB(prog);
	glGetObjectParameterivARB(prog, GL_OBJECT_LINK_STATUS_ARB, &linked);
	if(!linked) {
		fprintf(stderr, "shader linking failed\n");
		return 0;
	}

	glUseProgramObjectARB(prog);
	return prog;
}

void set_uniform1f(unsigned int prog, const char *name, float val) {
	int loc = glGetUniformLocationARB(prog, name);
	if(loc != -1) {
		glUniform1f(loc, val);
	} else {
		printf("OpenGL uniform error for name: '%s'\n", name);
	}
}

void set_uniform2f(unsigned int prog, const char *name, float v1, float v2) {
	int loc = glGetUniformLocationARB(prog, name);
	if(loc != -1) {
		glUniform2f(loc, v1, v2);
	} else {
		printf("OpenGL uniform error for name: '%s'\n", name);
	}
}
void set_uniform1i(unsigned int prog, const char *name, int val) {
        int loc = glGetUniformLocationARB(prog, name);
        if(loc != -1) {
            glUniform1i(loc, val);
        }else {
		printf("OpenGL uniform error for name: '%s'\n", name);

	}
}




